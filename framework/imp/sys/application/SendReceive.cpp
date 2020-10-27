#include "api/sys/comm/event/Event.hpp"
#include "api/sys/application/Context.hpp"
#include "api/sys/application/Process.hpp"
#include "api/sys/application/SendReceive.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "SendReceive"



using namespace base::application;



SendReceive::SendReceive( )
   : m_thread( std::bind( &SendReceive::thread_loop, this ) )
{
   mp_socket_sb = std::shared_ptr< os::Socket >(
      new os::Socket( Process::instance( )->configuration( ).ipc_sb, Process::instance( )->configuration( ).ipc_sb_buffer_size )
   );

   mp_socket_master = std::shared_ptr< os::Socket >(
      new os::Socket( Process::instance( )->configuration( ).ipc_app, Process::instance( )->configuration( ).ipc_app_buffer_size )
   );
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
   if( os::Socket::eResult::ERROR == mp_socket_sb->create( ) )
      return false;
   if( os::Socket::eResult::ERROR == mp_socket_sb->connect( ) )
      return false;
   mp_socket_sb->unblock( );
   mp_socket_sb->info( "ServiceBrocker connection created" );

   if( os::Socket::eResult::ERROR == mp_socket_master->create( ) )
      return false;
   if( os::Socket::eResult::ERROR == mp_socket_master->bind( ) )
      return false;
   mp_socket_master->unblock( );
   if( os::Socket::eResult::ERROR == mp_socket_master->listen( ) )
      return false;
   mp_socket_master->info( "Application connection created" );

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

   fd_set.set( mp_socket_sb->socket( ), os::linux::socket::fd::eType::READ );
   max_socket = mp_socket_sb->socket( );

   fd_set.set( mp_socket_master->socket( ), os::linux::socket::fd::eType::READ );
   if( mp_socket_master->socket( ) > max_socket )
      max_socket = mp_socket_master->socket( );

   for( const auto& p_socket : m_pending_sockets )
   {
      fd_set.set( p_socket->socket( ), os::linux::socket::fd::eType::READ );
      if( p_socket->socket( ) > max_socket )
         max_socket = p_socket->socket( );
   }
   SYS_INF( "max_socket = %d", max_socket );
}

void SendReceive::process_select( os::linux::socket::tSocket& max_socket, os::linux::socket::fd& fd_set )
{
   // process service brocker sockets
   if( true == fd_set.is_set( mp_socket_sb->socket( ), os::linux::socket::fd::eType::READ ) )
   {
      const os::Socket::eResult result = mp_socket_sb->recv( );
      if( os::Socket::eResult::DISCONNECTED == result )
      {
         mp_socket_sb->info( "Host disconnected" );
      }
      else if( os::Socket::eResult::OK == result )
      {
         size_t recv_size = 0;
         const void* const p_buffer = mp_socket_sb->buffer( recv_size );
         dsi::tByteStream stream;
         stream.push( p_buffer, recv_size );
         process_stream( stream, mp_socket_sb );
      }
   }

   // process client sockets
   for( auto iterator = m_pending_sockets.begin( ); iterator != m_pending_sockets.end( ); ++iterator )
   {
      auto p_socket = *iterator;
      if( false == fd_set.is_set( p_socket->socket( ), os::linux::socket::fd::eType::READ ) )
         continue;

      const os::Socket::eResult result = p_socket->recv( );
      if( os::Socket::eResult::DISCONNECTED == result )
      {
         p_socket->info( "Host disconnected" );
         iterator = m_pending_sockets.erase( iterator );
         if( m_pending_sockets.end( ) == iterator )
            break;
      }
      else if( os::Socket::eResult::OK == result )
      {
         size_t recv_size = 0;
         const void* const p_buffer = p_socket->buffer( recv_size );
         dsi::tByteStream stream;
         stream.push( p_buffer, recv_size );
         process_stream( stream, p_socket );
         // m_pending_sockets.erase( iterator ); // @TDA: should be removed if was processed and added to m_process_mapping
      }
   }

   // process master sockets
   if( true == fd_set.is_set( mp_socket_master->socket( ), os::linux::socket::fd::eType::READ ) )
   {
      if( auto p_socket = mp_socket_master->accept( ) )
      {
         m_pending_sockets.insert( p_socket );
         p_socket->info( "Host connected" );
         p_socket->unblock( );
      }
   }
}

bool SendReceive::process_stream( dsi::tByteStream& stream, os::Socket::tSptr p_socket_from )
{
   bool result = true;
   while( 0 < stream.size( ) )
   {
      dsi::Packet packet;
      stream.pop( packet );
      result &= process_packet( packet, p_socket_from );
   }
   return result;
}

bool SendReceive::process_packet( dsi::Packet& packet, os::Socket::tSptr p_socket_from )
{
   bool result = true;
   for( dsi::Package& package : packet.packages( ) )
      result &= process_package( package, p_socket_from );

   return result;
}

bool SendReceive::process_package( dsi::Package& package, os::Socket::tSptr p_socket_from )
{
   SYS_TRC( "Processing package '%s'", package.c_str( ) );

   switch( package.command( ) )
   {
      case dsi::eCommand::BroadcastEvent:
      {
         async::IEvent::tSptr p_event = async::IEvent::deserialize( package.data( ) );
         if( nullptr == p_event )
         {
            SYS_ERR( "lost received event" );
         }
         else
         {
            application::Context to_context;
            if( false == package.data( to_context ) )
            {
               SYS_ERR( "parce package error" );
               return false;
            }
            SYS_TRC( "received event '%s' to context: %s", p_event->signature( )->name( ).c_str( ), to_context.name( ).c_str( ) );
            p_event->send( to_context );
         }
         break;
      }
      case dsi::eCommand::DetectedServer:
      {
         service::Signature service_signature;
         service::Address service_address;
         dsi::SocketCongiguration inet_address;
         if( false == package.data( service_signature, service_address, inet_address ) )
         {
            SYS_ERR( "parce package error" );
            return false;
         }
         SYS_INF(
            "detected server: '%s' / %s / %s",
            service_signature.name( ).c_str( ), service_address.name( ).c_str( ), inet_address.name( ).c_str( )
         );

         os::Socket::tSptr p_socket = nullptr;

         // Check if socket to interested process was created before
         auto iterator_process_mapping = m_process_mapping.find( service_address.context( ).pid( ) );
         if( m_process_mapping.end( ) != iterator_process_mapping )
         {
            p_socket = iterator_process_mapping->second.socket;
         }
         else
         {
            auto iterator_pending_sockets = std::find_if(
               m_pending_sockets.begin( ),
               m_pending_sockets.end( ),
               [ &inet_address ]( const os::Socket::tSptr p_pending_socket )
               {
                  return p_pending_socket->configuration( ) == inet_address;
               }
            );
            if( m_pending_sockets.end( ) != iterator_pending_sockets )
            {
               p_socket = *iterator_pending_sockets;
            }
            else
            {
               p_socket = std::make_shared< os::Socket >( inet_address, Process::instance( )->configuration( ).ipc_app_buffer_size );
               if( os::Socket::eResult::ERROR == p_socket->create( ) )
                  return false;
               if( os::Socket::eResult::ERROR == p_socket->connect( ) )
                  return false;
               p_socket->unblock( );
               p_socket->info( "connection to process created" );
               m_pending_sockets.insert( p_socket );

               dsi::Packet packet( dsi::eCommand::RegisterProcess, application::Process::instance( )->id( ) );
               send( packet, p_socket );

               m_process_mapping.insert( std::make_pair( service_address.context( ).pid( ), ProcessInfo{ p_socket } ) );
            }
         }

         auto clients_addresses = application::Process::instance( )->service_registry( ).clients( service_signature );
         for( const auto& client_address : clients_addresses )
         {
            dsi::Packet packet( dsi::eCommand::RegisterClient, service_signature, client_address );
            send( packet, p_socket );
         }

         break;
      }
      case dsi::eCommand::DetectedClient:
      {
         service::Signature service_signature;
         service::Address service_address;
         dsi::SocketCongiguration inet_address;
         if( false == package.data( service_signature, service_address, inet_address ) )
         {
            SYS_ERR( "parce package error" );
            return false;
         }
         SYS_INF(
            "detected client: '%s' / %s / %s",
            service_signature.name( ).c_str( ), service_address.name( ).c_str( ), inet_address.name( ).c_str( )
         );

         break;
      }
      case dsi::eCommand::RegisterProcess:
      {
         application::process::ID pid;
         if( false == package.data( pid ) )
         {
            SYS_ERR( "parce package error" );
            return false;
         }
         SYS_INF( "register process: %s", pid.name( ).c_str( ) );

         auto iterator = m_pending_sockets.find( p_socket_from );
         if( m_pending_sockets.end( ) == iterator )
         {
            SYS_WRN( "there is no pending socket" );
         }

         m_process_mapping.insert( std::make_pair( pid, ProcessInfo{ p_socket_from } ) );

         break;
      }
      case dsi::eCommand::RegisterServer:
      {
         service::Signature service_signature;
         service::Address service_address;
         if( false == package.data( service_signature, service_address ) )
         {
            SYS_ERR( "parce package error" );
            return false;
         }
         SYS_INF( "register server: %s / %s", service_signature.name( ).c_str( ), service_address.name( ).c_str( ) );

         application::Process::instance( )->service_registry( ).register_server( service_signature, service_address );

         break;
      }
      case dsi::eCommand::RegisterClient:
      {
         service::Signature service_signature;
         service::Address service_address;
         if( false == package.data( service_signature, service_address ) )
         {
            SYS_ERR( "parce package error" );
            return false;
         }
         SYS_INF( "register client: %s / %s", service_signature.name( ).c_str( ), service_address.name( ).c_str( ) );

         const auto& server_address = application::Process::instance( )->service_registry( ).server( service_signature );
         dsi::Packet packet( dsi::eCommand::RegisterServer, service_signature, server_address );
         send( packet, p_socket_from );

         application::Process::instance( )->service_registry( ).register_client( service_signature, service_address );

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

base::os::Socket::tSptr SendReceive::socket( const application::Context& context )
{
   if( false == context.is_valid( ) )
      return mp_socket_sb;

   auto iterator = m_process_mapping.find( context.pid( ) );
   if( m_process_mapping.end( ) == iterator )
   {
      SYS_WRN( "unable to find socket for context '%s'", context.name( ).c_str( ) );
      return nullptr;
   }

   return iterator->second.socket;
}

bool SendReceive::send( const RawBuffer& buffer, os::Socket::tSptr p_socket )
{
   if( nullptr == buffer.ptr )
      return false;
   if( nullptr == p_socket )
      return false;

   return os::Socket::eResult::OK == p_socket->send( buffer.ptr, buffer.size );
}

bool SendReceive::send( const RawBuffer& buffer, const application::Context& to_context )
{
   return send( buffer, socket( to_context ) );
}

bool SendReceive::send( const dsi::Packet& packet, os::Socket::tSptr p_socket )
{
   RawBuffer buffer = dsi::tByteStream::serialize( packet );
   bool result = send( buffer, p_socket );
   buffer.free( );

   return result;
}

bool SendReceive::send( const dsi::Packet& packet, const application::Context& to_context )
{
   RawBuffer buffer = dsi::tByteStream::serialize( packet );
   bool result = send( buffer, socket( to_context ) );
   buffer.free( );

   return result;
}

bool SendReceive::send( const async::IEvent::tSptr p_event, const application::Context& to_context )
{
   dsi::Packet packet( dsi::eCommand::BroadcastEvent, *p_event, to_context );
   return send( packet, to_context );
}
