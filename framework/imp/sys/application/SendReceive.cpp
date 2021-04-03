#include "api/sys/comm/async/event/Event.hpp"
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
      new os::Socket( Process::instance( )->configuration( ).ipc_sb.socket, Process::instance( )->configuration( ).ipc_sb.buffer_size )
   );

   mp_socket_master = std::shared_ptr< os::Socket >(
      new os::Socket( Process::instance( )->configuration( ).ipc_app.socket, Process::instance( )->configuration( ).ipc_app.buffer_size )
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

   for( const auto& pair : m_pending_sockets )
   {
      auto p_socket = pair.first;
      fd_set.set( p_socket->socket( ), os::linux::socket::fd::eType::READ );
      if( p_socket->socket( ) > max_socket )
         max_socket = p_socket->socket( );
   }

   for( const auto& pair : m_process_mapping )
   {
      auto p_socket = pair.second.socket;
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
         mp_socket_sb->info( "ServiceBrocker disconnected" );
      }
      else if( os::Socket::eResult::OK == result )
      {
         size_t recv_size = 0;
         const void* const p_buffer = mp_socket_sb->buffer( recv_size );
         ipc::tStream stream;
         stream.push( p_buffer, recv_size );
         process_stream( stream, mp_socket_sb );
      }
   }

   // process client sockets
   for( auto iterator = m_process_mapping.begin( ); iterator != m_process_mapping.end( ); ++iterator )
   {
      auto p_socket = iterator->second.socket;
      if( false == fd_set.is_set( p_socket->socket( ), os::linux::socket::fd::eType::READ ) )
         continue;

      const os::Socket::eResult result = p_socket->recv( );
      if( os::Socket::eResult::DISCONNECTED == result )
      {
         p_socket->info( "Server disconnected" );

         for( const auto& service_passport : iterator->second.service_passport_list )
            application::Process::instance( )->service_registry( ).unregister_server( service_passport );

         iterator = m_process_mapping.erase( iterator );
         if( m_process_mapping.end( ) == iterator )
            break;
      }
      else if( os::Socket::eResult::OK == result )
      {
         SYS_WRN( "Received data from server" );
      }
   }

   // process client sockets
   for( auto iterator = m_pending_sockets.begin( ); iterator != m_pending_sockets.end( ); ++iterator )
   {
      auto p_socket = iterator->first;
      if( false == fd_set.is_set( p_socket->socket( ), os::linux::socket::fd::eType::READ ) )
         continue;

      const os::Socket::eResult result = p_socket->recv( );
      if( os::Socket::eResult::DISCONNECTED == result )
      {
         p_socket->info( "Client disconnected" );

         for( const auto& service_passport : iterator->second )
            application::Process::instance( )->service_registry( ).unregister_client( service_passport );

         iterator = m_pending_sockets.erase( iterator );
         if( m_pending_sockets.end( ) == iterator )
            break;
      }
      else if( os::Socket::eResult::OK == result )
      {
         size_t recv_size = 0;
         const void* const p_buffer = p_socket->buffer( recv_size );
         ipc::tStream stream;
         stream.push( p_buffer, recv_size );
         process_stream( stream, p_socket );
      }
   }

   // process master sockets
   if( true == fd_set.is_set( mp_socket_master->socket( ), os::linux::socket::fd::eType::READ ) )
   {
      if( auto p_socket = mp_socket_master->accept( ) )
      {
         m_pending_sockets.insert( std::make_pair( p_socket, service::Passport::tList{ } ) );
         p_socket->info( "Client connected" );
         p_socket->unblock( );
      }
   }
}

bool SendReceive::process_stream( ipc::tStream& stream, os::Socket::tSptr p_socket_from )
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
   SYS_VRB( "Processing package '%s'", package.c_str( ) );

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
            application::Context to_context = application::Context::internal_broadcast;
            if( false == package.data( to_context ) )
            {
               SYS_ERR( "parce package error" );
               return false;
            }
            SYS_VRB( "received event '%s' to context: %s", p_event->signature( )->name( ).c_str( ), to_context.name( ).c_str( ) );
            p_event->send( to_context );
         }
         break;
      }
      case dsi::eCommand::DetectedServer:
      {
         service::Passport service_passport;
         dsi::SocketCongiguration inet_address;
         if( false == package.data( service_passport, inet_address ) )
         {
            SYS_ERR( "parce package error" );
            return false;
         }
         SYS_INF( "detected server: '%s' / %s", service_passport.name( ).c_str( ), inet_address.name( ).c_str( ) );

         os::Socket::tSptr p_socket = nullptr;
         auto iterator_process_mapping = m_process_mapping.find( service_passport.address.context( ).pid( ) );
         if( m_process_mapping.end( ) != iterator_process_mapping )
         {
            p_socket = iterator_process_mapping->second.socket;
         }
         else
         {
            p_socket = std::make_shared< os::Socket >( inet_address, Process::instance( )->configuration( ).ipc_app.buffer_size );
            if( os::Socket::eResult::ERROR == p_socket->create( ) )
               return false;
            if( os::Socket::eResult::ERROR == p_socket->connect( ) )
               return false;
            p_socket->unblock( );
            p_socket->info( "connection to process created" );
            m_process_mapping.insert( std::make_pair( service_passport.address.context( ).pid( ), ProcessInfo( p_socket ) ) );

            dsi::Packet packet(
               dsi::eCommand::RegisterProcess,
               application::Process::instance( )->id( ),
               static_cast< dsi::SocketCongiguration >( Process::instance( )->configuration( ).ipc_app.socket )
            );
            send( packet, p_socket );
         }

         auto clients_addresses = application::Process::instance( )->service_registry( ).clients( service_passport );
         for( const auto& client_address : clients_addresses )
         {
            dsi::Packet packet( dsi::eCommand::RegisterClient, service::Passport( service_passport.signature, client_address ) );
            send( packet, p_socket );
         }

         break;
      }
      case dsi::eCommand::DetectedClient:
      {
         service::Passport service_passport;
         dsi::SocketCongiguration inet_address;
         if( false == package.data( service_passport, inet_address ) )
         {
            SYS_ERR( "parce package error" );
            return false;
         }
         SYS_INF( "detected client: %s / %s", service_passport.name( ).c_str( ), inet_address.name( ).c_str( ) );

         break;
      }
      case dsi::eCommand::RegisterProcess:
      {
         application::process::ID pid;
         dsi::SocketCongiguration inet_address;
         if( false == package.data( pid, inet_address ) )
         {
            SYS_ERR( "parce package error" );
            return false;
         }
         SYS_INF( "register process: %s / %s", pid.name( ).c_str( ), inet_address.name( ).c_str( ) );

         os::Socket::tSptr p_socket = nullptr;
         auto iterator_process_mapping = m_process_mapping.find( pid );
         if( m_process_mapping.end( ) != iterator_process_mapping )
         {
            SYS_WRN( "process already registered: %s", pid.name( ).c_str( ) );
            p_socket = iterator_process_mapping->second.socket;
         }
         else
         {
            p_socket = std::make_shared< os::Socket >( inet_address, Process::instance( )->configuration( ).ipc_app.buffer_size );
            if( os::Socket::eResult::ERROR == p_socket->create( ) )
               return false;
            if( os::Socket::eResult::ERROR == p_socket->connect( ) )
               return false;
            p_socket->unblock( );
            p_socket->info( "connection to process created" );
            m_process_mapping.insert( std::make_pair( pid, ProcessInfo( p_socket ) ) );
         }

         dsi::Packet packet( dsi::eCommand::RegisterProcessAck, application::Process::instance( )->id( ) );
         send( packet, p_socket );

         break;
      }
      case dsi::eCommand::RegisterProcessAck:
      {
         application::process::ID pid;
         if( false == package.data( pid ) )
         {
            SYS_ERR( "parce package error" );
            return false;
         }
         SYS_INF( "register process acknowledge: %s", pid.name( ).c_str( ) );

         auto iterator_process_mapping = m_process_mapping.find( pid );
         if( m_process_mapping.end( ) == iterator_process_mapping )
         {
            SYS_WRN( "acknowledge for unknown process" );
            return false;
         }

         iterator_process_mapping->second.confirmed = true;

         break;
      }
      case dsi::eCommand::RegisterServer:
      {
         service::Passport service_passport;
         if( false == package.data( service_passport ) )
         {
            SYS_ERR( "parce package error" );
            return false;
         }
         SYS_INF( "register server: %s", service_passport.name( ).c_str( ) );

         auto iterator_process_mapping = m_process_mapping.find( service_passport.address.context( ).pid( ) );
         if( m_process_mapping.end( ) == iterator_process_mapping )
         {
            SYS_WRN( "process was not registered: %s", service_passport.address.context( ).pid( ).name( ).c_str( ) );
            return false;
         }

         iterator_process_mapping->second.service_passport_list.push_back( service_passport );
         application::Process::instance( )->service_registry( ).register_server( service_passport );

         break;
      }
      case dsi::eCommand::RegisterClient:
      {
         service::Passport service_passport;
         if( false == package.data( service_passport ) )
         {
            SYS_ERR( "parce package error" );
            return false;
         }
         SYS_INF( "register client: %s", service_passport.name( ).c_str( ) );

         const auto& server_address = application::Process::instance( )->service_registry( ).server( service_passport );
         if( false == server_address.is_valid( ) )
         {
            SYS_WRN( "server is not registered" );
            return false;
         }

         auto iterator_process_mapping = m_process_mapping.find( service_passport.address.context( ).pid( ) );
         if( m_process_mapping.end( ) == iterator_process_mapping )
         {
            SYS_WRN( "process was not registered: %s", service_passport.address.context( ).pid( ).name( ).c_str( ) );
            return false;
         }
         os::Socket::tSptr p_socket = iterator_process_mapping->second.socket;

         dsi::Packet packet( dsi::eCommand::RegisterServer, service::Passport( service_passport.signature, server_address ) );
         send( packet, p_socket );

         auto iterator_pending_sockets = m_pending_sockets.find( p_socket_from );
         if( m_pending_sockets.end( ) == iterator_pending_sockets )
         {
            // This situation should never happen because current method is called during processing
            // sockets from m_pending_sockets
            SYS_WRN( "service was not maped to the socket" );
            return false;
         }
         iterator_pending_sockets->second.push_back( service_passport );

         application::Process::instance( )->service_registry( ).register_client( service_passport );

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
   if( context.pid( ).is_invalid( ) )
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
   RawBuffer buffer = ipc::tStream::serialize( packet );
   bool result = send( buffer, p_socket );
   buffer.free( );

   return result;
}

bool SendReceive::send( const dsi::Packet& packet, const application::Context& to_context )
{
   RawBuffer buffer = ipc::tStream::serialize( packet );
   bool result = send( buffer, socket( to_context ) );
   buffer.free( );

   return result;
}

bool SendReceive::send( const async::IEvent::tSptr p_event, const application::Context& to_context )
{
   dsi::Packet packet( dsi::eCommand::BroadcastEvent, *p_event, to_context );
   return send( packet, to_context );
}
