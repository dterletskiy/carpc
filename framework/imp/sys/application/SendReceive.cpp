#include "api/sys/comm/async/event/Event.hpp"
#include "api/sys/application/Context.hpp"
#include "api/sys/application/Process.hpp"
#include "imp/sys/application/SendReceive.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "SendReceive"



using namespace base::application;



SendReceive::SendReceive( )
   : m_thread( std::bind( &SendReceive::thread_loop, this ) )
   , m_service_brocker( *this )
   , m_master( *this )
   , m_pending_connections( *this )
   , m_connections( *this )
{
}

SendReceive::~SendReceive( )
{
}

bool SendReceive::start( )
{
   if( false == m_service_brocker.setup_connection( ) )
      return false;
   if( false == m_master.setup_connection( ) )
      return false;
   if( false == m_pending_connections.setup_connection( ) )
      return false;
   if( false == m_connections.setup_connection( ) )
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

void SendReceive::thread_loop( )
{
   SYS_INF( "enter" );
   m_started = true;

   os::os_linux::socket::fd fd_set;
   os::os_linux::socket::tSocket max_socket = os::os_linux::socket::InvalidSocket;

   while( started( ) )
   {
      fd_set.reset( );
      max_socket = os::os_linux::socket::InvalidSocket;
      m_service_brocker.prepare_select( max_socket, fd_set );
      m_master.prepare_select( max_socket, fd_set );
      m_connections.prepare_select( max_socket, fd_set );
      m_pending_connections.prepare_select( max_socket, fd_set );

      if( false == os::os_linux::socket::select( max_socket, fd_set ) )
         continue;

      // WARNING!!! This order is important because during processing some sockets could be moved from one collection to another.
      m_service_brocker.process_select( fd_set );
      m_connections.process_select( fd_set );
      m_pending_connections.process_select( fd_set );
      m_master.process_select( fd_set );
   }

   SYS_INF( "exit" );
}

base::os::Socket::tSptr SendReceive::socket( const application::Context& context )
{
   if( context.pid( ).is_invalid( ) )
      return m_service_brocker.mp_socket;

   auto& channel = m_connections.find_connection( context.pid( ) );
   if( nullptr == channel.mp_socket_send )
   {
      SYS_WRN( "unable to find socket for context '%s'", context.name( ).c_str( ) );
   }

   return channel.mp_socket_send;
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
   dsi::Packet packet( dsi::eCommand::IpcEvent, *p_event, to_context );
   return send( packet, to_context );
}





SendReceive::Base::Base( SendReceive& parent )
   : m_parent( parent )
{
}

bool SendReceive::Base::process_stream( ipc::tStream& stream )
{
   bool result = true;
   while( 0 < stream.size( ) )
   {
      dsi::Packet packet;
      stream.pop( packet );
      result &= process_packet( packet );
   }
   return result;
}

bool SendReceive::Base::process_packet( dsi::Packet& packet )
{
   bool result = true;
   for( dsi::Package& package : packet.packages( ) )
      result &= process_package( package );

   return result;
}



SendReceive::ServiceBrocker::ServiceBrocker( SendReceive& parent )
   : Base( parent )
{
}

bool SendReceive::ServiceBrocker::setup_connection( )
{
   mp_socket = os::Socket::create_shared(
      Process::instance( )->configuration( ).ipc_sb.socket, Process::instance( )->configuration( ).ipc_sb.buffer_size
   );

   if( nullptr == mp_socket )
      return false;

   if( os::Socket::eResult::ERROR == mp_socket->create( ) )
      return false;
   if( os::Socket::eResult::ERROR == mp_socket->connect( ) )
      return false;
   mp_socket->unblock( );
   mp_socket->info( "ServiceBrocker connection created" );

   return true;
}

void SendReceive::ServiceBrocker::prepare_select( os::os_linux::socket::tSocket& max_socket, os::os_linux::socket::fd& fd_set )
{
   fd_set.set( mp_socket->socket( ), os::os_linux::socket::fd::eType::READ );
   if( mp_socket->socket( ) > max_socket )
      max_socket = mp_socket->socket( );   
}

void SendReceive::ServiceBrocker::process_select( os::os_linux::socket::fd& fd_set )
{
   if( false == fd_set.is_set( mp_socket->socket( ), os::os_linux::socket::fd::eType::READ ) )
      return;

   const os::Socket::eResult result = mp_socket->recv( );
   if( os::Socket::eResult::DISCONNECTED == result )
   {
      mp_socket->info( "ServiceBrocker disconnected" );
   }
   else if( os::Socket::eResult::OK == result )
   {
      size_t recv_size = 0;
      const void* const p_buffer = mp_socket->buffer( recv_size );
      ipc::tStream stream;
      stream.push( p_buffer, recv_size );
      process_stream( stream );
   }
}

bool SendReceive::ServiceBrocker::process_package( dsi::Package& package )
{
   SYS_VRB( "Processing package '%s'", package.c_str( ) );

   switch( package.command( ) )
   {
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

         auto& pid = service_passport.address.context( ).pid( );
         auto& channel = m_parent.m_connections.register_connection( pid );

         if( nullptr == channel.mp_socket_send )
         {
            channel.mp_socket_send = os::Socket::create_shared( inet_address, Process::instance( )->configuration( ).ipc_app.buffer_size );
            if( os::Socket::eResult::ERROR == channel.mp_socket_send->create( ) )
               return false;
            if( os::Socket::eResult::ERROR == channel.mp_socket_send->connect( ) )
               return false;
            channel.mp_socket_send->unblock( );
            channel.mp_socket_send->info( "connection to process created" );

            dsi::Packet packet(
               dsi::eCommand::RegisterProcess,
               application::Process::instance( )->id( ),
               static_cast< dsi::SocketCongiguration >( Process::instance( )->configuration( ).ipc_app.socket )
            );
            m_parent.send( packet, channel.mp_socket_send );

            channel.pending_server_service_list.push_back( service_passport );
         }
         else
         {
            auto clients_addresses = application::Process::instance( )->service_registry( ).clients( service_passport );
            for( const auto& client_address : clients_addresses )
            {
               dsi::Packet packet( dsi::eCommand::RegisterClient, service::Passport( service_passport.signature, client_address ) );
               m_parent.send( packet, channel.mp_socket_send );
            }
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
      default:
      {
         SYS_WRN( "Unknown package command" );
         break;
      }
   }

   return true;
}



SendReceive::Master::Master( SendReceive& parent )
   : Base( parent )
{
}

bool SendReceive::Master::setup_connection( )
{
   mp_socket = os::Socket::create_shared(
      Process::instance( )->configuration( ).ipc_app.socket, Process::instance( )->configuration( ).ipc_app.buffer_size
   );

   if( nullptr == mp_socket )
      return false;

   if( os::Socket::eResult::ERROR == mp_socket->create( ) )
      return false;
   if( os::Socket::eResult::ERROR == mp_socket->bind( ) )
      return false;
   mp_socket->unblock( );
   if( os::Socket::eResult::ERROR == mp_socket->listen( ) )
      return false;
   mp_socket->info( "Application connection created" );

   return true;
}

void SendReceive::Master::prepare_select( os::os_linux::socket::tSocket& max_socket, os::os_linux::socket::fd& fd_set )
{
   fd_set.set( mp_socket->socket( ), os::os_linux::socket::fd::eType::READ );
   if( mp_socket->socket( ) > max_socket )
      max_socket = mp_socket->socket( );   
}

void SendReceive::Master::process_select( os::os_linux::socket::fd& fd_set )
{
   if( false == fd_set.is_set( mp_socket->socket( ), os::os_linux::socket::fd::eType::READ ) )
      return;

   if( auto p_socket = mp_socket->accept( ) )
   {
      m_parent.m_pending_connections.add( p_socket );
      p_socket->info( "Client connected" );
      p_socket->unblock( );
   }
}

bool SendReceive::Master::process_package( dsi::Package& package )
{
   SYS_VRB( "Processing package '%s'", package.c_str( ) );

   switch( package.command( ) )
   {
      default:
      {
         SYS_WRN( "Unknown package command" );
         break;
      }
   }

   return true;
}



SendReceive::PendingConnections::PendingConnections( SendReceive& parent )
   : Base( parent )
{
}

bool SendReceive::PendingConnections::setup_connection( )
{
   return true;
}

void SendReceive::PendingConnections::prepare_select( os::os_linux::socket::tSocket& max_socket, os::os_linux::socket::fd& fd_set )
{
   for( const auto& p_socket : m_sockets )
   {
      fd_set.set( p_socket->socket( ), os::os_linux::socket::fd::eType::READ );
      if( p_socket->socket( ) > max_socket )
         max_socket = p_socket->socket( );
   }
}

void SendReceive::PendingConnections::process_select( os::os_linux::socket::fd& fd_set )
{
   for( m_sockets_iterator = m_sockets.begin( ); m_sockets_iterator != m_sockets.end( ); ++m_sockets_iterator )
   {
      auto p_socket = *m_sockets_iterator;
      if( false == fd_set.is_set( p_socket->socket( ), os::os_linux::socket::fd::eType::READ ) )
         continue;

      const os::Socket::eResult result = p_socket->recv( );
      if( os::Socket::eResult::DISCONNECTED == result )
      {
         p_socket->info( "Client disconnected" );

         m_sockets_iterator = m_sockets.erase( m_sockets_iterator );
      }
      else if( os::Socket::eResult::OK == result )
      {
         size_t recv_size = 0;
         const void* const p_buffer = p_socket->buffer( recv_size );
         ipc::tStream stream;
         stream.push( p_buffer, recv_size );
         process_stream( stream );
      }

      if( m_sockets.end( ) == m_sockets_iterator )
         break;
   }
}

bool SendReceive::PendingConnections::process_package( dsi::Package& package )
{
   SYS_VRB( "Processing package '%s'", package.c_str( ) );

   switch( package.command( ) )
   {
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

         auto& channel = m_parent.m_connections.register_connection( pid );

         if( nullptr == channel.mp_socket_send )
         {
            channel.mp_socket_send = os::Socket::create_shared( inet_address, Process::instance( )->configuration( ).ipc_app.buffer_size );
            if( os::Socket::eResult::ERROR == channel.mp_socket_send->create( ) )
               return false;
            if( os::Socket::eResult::ERROR == channel.mp_socket_send->connect( ) )
               return false;
            channel.mp_socket_send->unblock( );
            channel.mp_socket_send->info( "connection to process created" );
         }

         channel.mp_socket_recv = *m_sockets_iterator;
         m_sockets_iterator = m_sockets.erase( m_sockets_iterator );

         dsi::Packet packet( dsi::eCommand::RegisterProcessAck, application::Process::instance( )->id( ) );
         m_parent.send( packet, channel.mp_socket_send );

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

         auto& channel = m_parent.m_connections.find_connection( pid );
         if( nullptr == channel.mp_socket_send )
         {
            SYS_WRN( "process '%s' was not registered or send socket is nullptr for connection to process", pid.name( ).c_str( ) );
            return false;
         }

         channel.mp_socket_recv = *m_sockets_iterator;
         m_sockets_iterator = m_sockets.erase( m_sockets_iterator );

         for( auto& pending_server_service_passport : channel.pending_server_service_list )
         {
            auto clients_addresses = application::Process::instance( )->service_registry( ).clients( pending_server_service_passport );
            for( const auto& client_address : clients_addresses )
            {
               dsi::Packet packet( dsi::eCommand::RegisterClient, service::Passport( pending_server_service_passport.signature, client_address ) );
               m_parent.send( packet, channel.mp_socket_send );
            }
         }
         channel.pending_server_service_list.clear( );

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

void SendReceive::PendingConnections::add( os::Socket::tSptr& p_socket )
{
   m_sockets.push_back( p_socket );
}



SendReceive::Connections::Connections( SendReceive& parent )
   : Base( parent )
{
}

bool SendReceive::Connections::setup_connection( )
{
   return true;
}

void SendReceive::Connections::prepare_select( os::os_linux::socket::tSocket& max_socket, os::os_linux::socket::fd& fd_set )
{
   for( const auto& pair : m_process_channel_map )
   {
      auto p_socket_recv = pair.second.mp_socket_recv;
      if( nullptr == p_socket_recv )
         continue;

      fd_set.set( p_socket_recv->socket( ), os::os_linux::socket::fd::eType::READ );
      if( p_socket_recv->socket( ) > max_socket )
         max_socket = p_socket_recv->socket( );
   }
}

void SendReceive::Connections::process_select( os::os_linux::socket::fd& fd_set )
{
   for( m_process_channel_map_iterator = m_process_channel_map.begin( ); m_process_channel_map_iterator != m_process_channel_map.end( ); ++m_process_channel_map_iterator )
   {
      auto p_socket_recv = m_process_channel_map_iterator->second.mp_socket_recv;
      if( nullptr == p_socket_recv )
         continue;

      if( false == fd_set.is_set( p_socket_recv->socket( ), os::os_linux::socket::fd::eType::READ ) )
         continue;

      const os::Socket::eResult result = p_socket_recv->recv( );
      if( os::Socket::eResult::DISCONNECTED == result )
      {
         p_socket_recv->info( "Server disconnected" );

         for( const auto& service_passport : m_process_channel_map_iterator->second.server_service_list )
            application::Process::instance( )->service_registry( ).unregister_server( service_passport );

         for( const auto& service_passport : m_process_channel_map_iterator->second.client_service_list )
            application::Process::instance( )->service_registry( ).unregister_client( service_passport );

         m_process_channel_map_iterator = m_process_channel_map.erase( m_process_channel_map_iterator );
      }
      else if( os::Socket::eResult::OK == result )
      {
         size_t recv_size = 0;
         const void* const p_buffer = p_socket_recv->buffer( recv_size );
         ipc::tStream stream;
         stream.push( p_buffer, recv_size );
         process_stream( stream );
      }

      if( m_process_channel_map.end( ) == m_process_channel_map_iterator )
         break;
   }
}

bool SendReceive::Connections::process_package( dsi::Package& package )
{
   SYS_VRB( "Processing package '%s'", package.c_str( ) );

   switch( package.command( ) )
   {
      case dsi::eCommand::IpcEvent:
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
      case dsi::eCommand::RegisterServer:
      {
         service::Passport server_passport;
         if( false == package.data( server_passport ) )
         {
            SYS_ERR( "parce package error" );
            return false;
         }
         SYS_INF( "register server: %s", server_passport.name( ).c_str( ) );

         auto& channel = m_process_channel_map_iterator->second;

         if( false == channel.established( ) )
         {
            SYS_WRN( "connection was not established to process %s", m_process_channel_map_iterator->first.name( ).c_str( ) );
            return false;
         }

         channel.server_service_list.push_back( server_passport );
         application::Process::instance( )->service_registry( ).register_server( server_passport );

         break;
      }
      case dsi::eCommand::RegisterClient:
      {
         service::Passport client_passport;
         if( false == package.data( client_passport ) )
         {
            SYS_ERR( "parce package error" );
            return false;
         }
         SYS_INF( "register client: %s", client_passport.name( ).c_str( ) );

         auto& channel = m_process_channel_map_iterator->second;

         if( false == channel.established( ) )
         {
            SYS_WRN( "connection was not established to process %s", m_process_channel_map_iterator->first.name( ).c_str( ) );
            return false;
         }

         const auto& server_address = application::Process::instance( )->service_registry( ).server( client_passport );
         if( false == server_address.is_valid( ) )
         {
            SYS_WRN( "server '%s' is not registered", client_passport.name( ).c_str( ) );
            return false;
         }
         service::Passport server_passport( client_passport.signature, server_address );

         dsi::Packet packet( dsi::eCommand::RegisterServer, server_passport );
         m_parent.send( packet, channel.mp_socket_send );

         channel.client_service_list.push_back( client_passport );
         application::Process::instance( )->service_registry( ).register_client( client_passport );

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
