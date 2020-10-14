#include "api/sys/comm/event/Event.hpp"
#include "api/sys/application/Context.hpp"
#include "api/sys/application/Process.hpp"
#include "api/sys/application/SendReceive.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "SendReceive"



using namespace base::application;



SendReceive::SendReceive( )
   : m_thread( std::bind( &SendReceive::thread_loop, this ) )
   , m_socket_sb( Process::instance( )->configuration( ).ipc_sb, Process::instance( )->configuration( ).ipc_sb_buffer_size )
   , m_socket_master( Process::instance( )->configuration( ).ipc_app, Process::instance( )->configuration( ).ipc_app_buffer_size )
{
}

SendReceive::~SendReceive( )
{
}

bool SendReceive::start( )
{
   if( false == setup_connection( ) )
      return false;

   SYS_INF( "starting recieve thread" );
   if( false == m_thread.run( ) )
   {
      SYS_ERR( "receive thread can't be started" );
      return false;
   }
   return true;
}

void SendReceive::stop( )
{
   SYS_INF( "stopping" );
   m_started = false;
}

bool SendReceive::setup_connection( )
{
   if( os::Socket::eResult::ERROR == m_socket_sb.create( ) )
      return false;
   if( os::Socket::eResult::ERROR == m_socket_sb.connect( ) )
      return false;
   m_socket_sb.unblock( );
   m_socket_sb.info( "ServiceBrocker connection created" );

   if( os::Socket::eResult::ERROR == m_socket_master.create( ) )
      return false;
   if( os::Socket::eResult::ERROR == m_socket_master.bind( ) )
      return false;
   m_socket_master.unblock( );
   if( os::Socket::eResult::ERROR == m_socket_master.listen( ) )
      return false;
   m_socket_master.info( "Application connection created" );

   return true;
}

void SendReceive::thread_loop( )
{
   SYS_INF( "enter" );
   m_started = true;

   os::linux::socket::fd fd_set;
   os::linux::socket::tSocket max_socket = os::linux::socket::InvalidSocket;

   while( started( ) )
   {
      prepare_select( max_socket, fd_set );

      if( false == os::linux::socket::select( max_socket, fd_set ) )
         continue;

      process_select( max_socket, fd_set );
   }

   SYS_INF( "exit" );
}

void SendReceive::prepare_select( os::linux::socket::tSocket& max_socket, os::linux::socket::fd& fd_set )
{
   fd_set.reset( );

   fd_set.set( m_socket_sb.socket( ), os::linux::socket::fd::eType::READ );
   max_socket = m_socket_sb.socket( );

   fd_set.set( m_socket_master.socket( ), os::linux::socket::fd::eType::READ );
   if( m_socket_master.socket( ) > max_socket )
      max_socket = m_socket_master.socket( );

   for( const auto& p_slave_socket : m_sockets_slave )
   {
      fd_set.set( p_slave_socket->socket( ), os::linux::socket::fd::eType::READ );
      if( p_slave_socket->socket( ) > max_socket )
         max_socket = p_slave_socket->socket( );
   }

   for( const auto& p_client_socket : m_sockets_client )
   {
      fd_set.set( p_client_socket->socket( ), os::linux::socket::fd::eType::READ );
      if( p_client_socket->socket( ) > max_socket )
         max_socket = p_client_socket->socket( );
   }
   SYS_INF( "max_socket = %d", max_socket );
}

void SendReceive::process_select( os::linux::socket::tSocket& max_socket, os::linux::socket::fd& fd_set )
{
   // process service brocker sockets
   if( true == fd_set.is_set( m_socket_sb.socket( ), os::linux::socket::fd::eType::READ ) )
   {
      const os::Socket::eResult result = m_socket_sb.recv( );
      if( os::Socket::eResult::DISCONNECTED == result )
      {
         m_socket_sb.info( "Host disconnected" );
      }
      else if( os::Socket::eResult::OK == result )
      {
         size_t recv_size = 0;
         const void* const p_buffer = m_socket_sb.buffer( recv_size );
         dsi::tByteStream stream;
         stream.push( p_buffer, recv_size );
         process_stream( stream, m_socket_sb );
      }
   }

   // process slave sockets
   for( auto iterator = m_sockets_slave.begin( ); iterator != m_sockets_slave.end( ); ++iterator )
   {
      if( false == fd_set.is_set( (*iterator)->socket( ), os::linux::socket::fd::eType::READ ) )
         continue;

      const os::Socket::eResult result = (*iterator)->recv( );
      if( os::Socket::eResult::DISCONNECTED == result )
      {
         (*iterator)->info( "Host disconnected" );
         iterator = m_sockets_slave.erase( iterator );
         if( m_sockets_slave.end( ) == iterator )
            break;
      }
      else if( os::Socket::eResult::OK == result )
      {
         size_t recv_size = 0;
         const void* const p_buffer = (*iterator)->buffer( recv_size );
         dsi::tByteStream stream;
         stream.push( p_buffer, recv_size );
         process_stream( stream, *(*iterator) );
      }
   }

   // process client sockets
   for( auto iterator = m_sockets_client.begin( ); iterator != m_sockets_client.end( ); ++iterator )
   {
      if( false == fd_set.is_set( (*iterator)->socket( ), os::linux::socket::fd::eType::READ ) )
         continue;

      const os::Socket::eResult result = (*iterator)->recv( );
      if( os::Socket::eResult::DISCONNECTED == result )
      {
         (*iterator)->info( "Host disconnected" );
         iterator = m_sockets_client.erase( iterator );
         if( m_sockets_client.end( ) == iterator )
            break;
      }
      else if( os::Socket::eResult::OK == result )
      {
         size_t recv_size = 0;
         const void* const p_buffer = (*iterator)->buffer( recv_size );
         dsi::tByteStream stream;
         stream.push( p_buffer, recv_size );
         process_stream( stream, *(*iterator) );
      }
   }

   // process master sockets
   if( true == fd_set.is_set( m_socket_master.socket( ), os::linux::socket::fd::eType::READ ) )
   {
      if( auto p_socket = m_socket_master.accept( ) )
      {
         m_sockets_slave.push_back( p_socket );
         p_socket->info( "Host connected" );
         p_socket->unblock( );
      }
   }
}

bool SendReceive::process_stream( dsi::tByteStream& stream, os::Socket& socket )
{
   while( 0 < stream.size( ) )
   {
      dsi::Packet packet;
      stream.pop( packet );
      process_packet( packet, socket );
   }
}

bool SendReceive::process_packet( dsi::Packet& packet, os::Socket& socket )
{
   bool result = true;
   for( dsi::Package& package : packet.packages( ) )
      result &= process_package( package, socket );

   return result;
}

bool SendReceive::process_package( dsi::Package& package, os::Socket& socket )
{
   SYS_TRC( "Processing package '%s'", package.c_str( ) );

   switch( package.command( ) )
   {
      case dsi::eCommand::BroadcastEvent:
      {
         base::async::IEvent::tSptr p_event = base::async::IEvent::deserialize( package.data( ) );
         if( nullptr == p_event )
         {
            SYS_ERR( "lost received event" );
         }
         else
         {
            SYS_TRC( "received event (%s)", p_event->signature( )->name( ).c_str( ) );
            p_event->send( Context( ) );
         }
         break;
      }
      default:
      {
         SYS_WRN( "Unknown package command" );
         break;
      }
   }

   return true;
}

bool SendReceive::send( const application::Context& to_context, const base::RawBuffer& buffer )
{
   return os::Socket::eResult::OK == m_socket_sb.send( buffer.ptr, buffer.size );
}
