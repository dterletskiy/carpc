#include "carpc/comm/async/event/Event.hpp"
#include "carpc/application/Context.hpp"
#include "carpc/application/Process.hpp"
#include "SendReceive.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "SendReceive"



using namespace carpc::application;



SendReceive::SendReceive( )
   : m_thread( std::bind( &SendReceive::thread_loop, this ) )
   , m_service_brocker( *this )
   , m_master( *this )
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
   m_started.store( false );
}

void SendReceive::thread_loop( )
{
   SYS_INF( "enter" );
   m_started.store( true );

   os::os_linux::socket::fds fd_set;
   os::os_linux::socket::tSocket max_socket = os::os_linux::socket::InvalidSocket;

   while( m_started.load( ) )
   {
      fd_set.reset( );
      max_socket = os::os_linux::socket::InvalidSocket;
      m_service_brocker.prepare_select( max_socket, fd_set );
      m_master.prepare_select( max_socket, fd_set );
      m_connections.prepare_select( max_socket, fd_set );

      timeval timeout{ 1, 0 };
      if( false == os::os_linux::socket::select( max_socket, fd_set, &timeout ) )
         continue;

      // @TDA: WARNING!!!
      // This order is important because during processing some sockets could be moved from one collection (master)
      // to another (connections). In this case connections collection is extended and will be processed in wrong way
      // if it would be processed after "master".
      m_service_brocker.process_select( fd_set );
      m_connections.process_select( fd_set );
      m_master.process_select( fd_set );
   }

   SYS_INF( "exit" );
}

carpc::os::Socket::tSptr SendReceive::socket( const application::Context& context )
{
   if( context.pid( ).is_invalid( ) )
      return m_service_brocker.mp_socket;

   auto p_socket_send = Connections::channel::send::socket( context.pid( ) );
   if( nullptr == p_socket_send )
   {
      SYS_WRN( "unable to find socket for context '%s'", context.dbg_name( ).c_str( ) );
   }

   return p_socket_send;
}

bool SendReceive::send( const void* const p_buffer, const std::size_t size, os::Socket::tSptr p_socket )
{
   if( nullptr == p_buffer )
      return false;
   if( nullptr == p_socket )
      return false;

   return os::Socket::eResult::OK == p_socket->send( p_buffer, size );
}

bool SendReceive::send( const ipc::tStream& stream, os::Socket::tSptr p_socket )
{
   return send( stream.buffer( ), stream.size( ), p_socket );
}

bool SendReceive::send( const ipc::Packet& packet, os::Socket::tSptr p_socket )
{
   ipc::tStream stream;
   ipc::serialize( stream, packet );
   return send( stream, p_socket );
}

bool SendReceive::send( const ipc::Packet& packet, const application::Context& to_context )
{
   return send( packet, socket( to_context ) );
}

bool SendReceive::send( const async::IEvent::tSptr p_event, const application::Context& to_context )
{
   ipc::Packet packet( ipc::eCommand::IpcEvent, *p_event, to_context );
   return send( packet, to_context );
}





SendReceive::Base::Base( SendReceive& parent )
   : m_parent( parent )
{
}

bool SendReceive::Base::process_stream( ipc::tStream& stream, os::Socket::tSptr p_socket )
{
   bool result = true;
   while( 0 < stream.size( ) )
   {
      ipc::Packet packet;
      ipc::deserialize( stream, packet );
      result &= process_packet( packet, p_socket );
   }
   return result;
}

bool SendReceive::Base::process_packet( ipc::Packet& packet, os::Socket::tSptr p_socket )
{
   bool result = true;
   for( ipc::Package& package : packet.packages( ) )
      result &= process_package( package, p_socket );

   return result;
}



SendReceive::ServiceBrocker::ServiceBrocker( SendReceive& parent )
   : Base( parent )
{
}

bool SendReceive::ServiceBrocker::setup_connection( )
{
   mp_socket = os::Socket::create_shared(
      configuration::current( ).ipc_sb.socket, configuration::current( ).ipc_sb.buffer_size
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

void SendReceive::ServiceBrocker::prepare_select( os::os_linux::socket::tSocket& max_socket, os::os_linux::socket::fds& fd_set )
{
   fd_set.set( mp_socket->socket( ), os::os_linux::socket::fds::eType::READ );
   if( mp_socket->socket( ) > max_socket )
      max_socket = mp_socket->socket( );   
}

void SendReceive::ServiceBrocker::process_select( os::os_linux::socket::fds& fd_set )
{
   if( false == fd_set.is_set( mp_socket->socket( ), os::os_linux::socket::fds::eType::READ ) )
      return;

   const os::Socket::eResult result = mp_socket->recv( );
   if( os::Socket::eResult::DISCONNECTED == result )
   {
      mp_socket->info( "ServiceBrocker disconnected" );
   }
   else if( os::Socket::eResult::OK == result )
   {
      std::size_t recv_size = 0;
      const void* const p_buffer = mp_socket->buffer( recv_size );
      ipc::tStream stream( p_buffer, recv_size );
      process_stream( stream, mp_socket );
   }
}

bool SendReceive::ServiceBrocker::process_package( ipc::Package& package, os::Socket::tSptr p_socket )
{
   SYS_VRB( "Processing package '%s'", package.c_str( ) );

   switch( package.command( ) )
   {
      case ipc::eCommand::DetectedServer:
      {
         service::Passport service_passport;
         ipc::SocketCongiguration inet_address;
         if( false == package.data( service_passport, inet_address ) )
         {
            SYS_ERR( "parce package error" );
            return false;
         }
         SYS_INF( "detected server: '%s' / %s", service_passport.dbg_name( ).c_str( ), inet_address.dbg_name( ).c_str( ) );

         auto& pid = service_passport.address.context( ).pid( );
         auto p_socket_send = Connections::channel::send::socket( pid );

         if( nullptr == p_socket_send )
         {
            p_socket_send = Connections::channel::send::create( pid, inet_address );
            if( nullptr == p_socket_send )
               return false;

            ipc::Packet packet(
               ipc::eCommand::RegisterProcess,
               application::process::current_id( ),
               static_cast< ipc::SocketCongiguration >( configuration::current( ).ipc_app.socket )
            );
            m_parent.send( packet, p_socket_send );

            Connections::interface::server::pending::add( pid, service_passport );
         }
         else
         {
            auto clients_addresses = application::Process::instance( )->service_registry( ).clients( service_passport );
            for( const auto& client_address : clients_addresses )
            {
               ipc::Packet packet( ipc::eCommand::RegisterClient, service::Passport( service_passport.signature, client_address ) );
               m_parent.send( packet, p_socket_send );
            }
         }

         break;
      }
      case ipc::eCommand::DetectedClient:
      {
         service::Passport service_passport;
         ipc::SocketCongiguration inet_address;
         if( false == package.data( service_passport, inet_address ) )
         {
            SYS_ERR( "parce package error" );
            return false;
         }
         SYS_INF( "detected client: %s / %s", service_passport.dbg_name( ).c_str( ), inet_address.dbg_name( ).c_str( ) );

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
      configuration::current( ).ipc_app.socket, configuration::current( ).ipc_app.buffer_size
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

void SendReceive::Master::prepare_select( os::os_linux::socket::tSocket& max_socket, os::os_linux::socket::fds& fd_set )
{
   fd_set.set( mp_socket->socket( ), os::os_linux::socket::fds::eType::READ );
   if( mp_socket->socket( ) > max_socket )
      max_socket = mp_socket->socket( );   
}

void SendReceive::Master::process_select( os::os_linux::socket::fds& fd_set )
{
   if( false == fd_set.is_set( mp_socket->socket( ), os::os_linux::socket::fds::eType::READ ) )
      return;

   if( auto p_socket = mp_socket->accept( ) )
   {
      Connections::channel::recv::add( p_socket );
      p_socket->info( "Client connected" );
      p_socket->unblock( );
   }
}

bool SendReceive::Master::process_package( ipc::Package& package, os::Socket::tSptr p_socket )
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



SendReceive::Connections::Connections( SendReceive& parent )
   : Base( parent )
{
}

bool SendReceive::Connections::setup_connection( )
{
   return true;
}

void SendReceive::Connections::prepare_select( os::os_linux::socket::tSocket& max_socket, os::os_linux::socket::fds& fd_set )
{
   for( const auto& pair : channel::recv::collection( ) )
   {
      auto p_socket = pair.first;
      if( nullptr == p_socket )
         continue;

      fd_set.set( p_socket->socket( ), os::os_linux::socket::fds::eType::READ );
      if( p_socket->socket( ) > max_socket )
         max_socket = p_socket->socket( );
   }
}

void SendReceive::Connections::process_select( os::os_linux::socket::fds& fd_set )
{
   auto iterator = channel::recv::collection( ).begin( );
   while( iterator != channel::recv::collection( ).end( ) )
   {
      auto p_socket_recv = iterator->first;
      auto& pid = iterator->second;
      if( nullptr == p_socket_recv )
      {
         ++iterator;
         continue;
      }

      if( false == fd_set.is_set( p_socket_recv->socket( ), os::os_linux::socket::fds::eType::READ ) )
      {
         ++iterator;
         continue;
      }

      const os::Socket::eResult result = p_socket_recv->recv( );
      if( os::Socket::eResult::DISCONNECTED == result )
      {
         p_socket_recv->info( "Server disconnected" );

         for( const auto& passport : interface::server::passports( pid ) )
            application::Process::instance( )->service_registry( ).unregister_server( passport );

         for( const auto& passport : interface::client::passports( pid ) )
            application::Process::instance( )->service_registry( ).unregister_client( passport );

         interface::server::remove( pid );
         interface::client::remove( pid );
         channel::send::remove( pid );
         iterator = channel::recv::collection( ).erase( iterator ); // @TDA: improve
      }
      else if( os::Socket::eResult::OK == result )
      {
         std::size_t recv_size = 0;
         const void* const p_buffer = p_socket_recv->buffer( recv_size );
         ipc::tStream stream( p_buffer, recv_size );
         process_stream( stream, p_socket_recv );
         ++iterator;
      }
      else
      {
         ++iterator;
      }
   }
}

bool SendReceive::Connections::process_package( ipc::Package& package, os::Socket::tSptr p_socket )
{
   SYS_VRB( "Processing package '%s'", package.c_str( ) );

   switch( package.command( ) )
   {
      case ipc::eCommand::RegisterProcess:
      {
         application::process::ID pid;
         ipc::SocketCongiguration inet_address;
         if( false == package.data( pid, inet_address ) )
         {
            SYS_ERR( "parce package error" );
            return false;
         }
         SYS_INF( "register process: %s / %s", pid.dbg_name( ).c_str( ), inet_address.dbg_name( ).c_str( ) );

         auto p_socket_send = channel::send::socket( pid );
         if( nullptr == p_socket_send )
         {
            p_socket_send = channel::send::create( pid, inet_address );
            if( nullptr == p_socket_send )
               return false;
         }

         channel::recv::update( p_socket, pid );

         ipc::Packet packet( ipc::eCommand::RegisterProcessAck, application::process::current_id( ) );
         m_parent.send( packet, p_socket_send );

         break;
      }
      case ipc::eCommand::RegisterProcessAck:
      {
         application::process::ID pid;
         if( false == package.data( pid ) )
         {
            SYS_ERR( "parce package error" );
            return false;
         }
         SYS_INF( "register process acknowledge: %s", pid.dbg_name( ).c_str( ) );

         auto p_socket_send = channel::send::socket( pid );
         if( nullptr == p_socket_send )
         {
            SYS_WRN( "process '%s' was not registered or send socket is nullptr for connection to process", pid.dbg_name( ).c_str( ) );
            return false;
         }

         channel::recv::update( p_socket, pid );

         for( auto& passport : interface::server::pending::passports( pid ) )
         {
            auto clients_addresses = application::Process::instance( )->service_registry( ).clients( passport );
            for( const auto& client_address : clients_addresses )
            {
               ipc::Packet packet( ipc::eCommand::RegisterClient, service::Passport( passport.signature, client_address ) );
               m_parent.send( packet, p_socket_send );
            }
         }
         interface::server::pending::remove( pid );

         break;
      }
      case ipc::eCommand::IpcEvent:
      {
         async::IEvent::tSptr p_event = async::IEvent::deserialize( package.data( ) );
         if( nullptr == p_event )
         {
            SYS_ERR( "lost received event" );
            return false;
         }

         application::Context to_context = application::Context::internal_broadcast;
         if( false == package.data( to_context ) )
         {
            SYS_ERR( "parce package error" );
            return false;
         }
         SYS_VRB( "received event '%s' to context: %s", p_event->signature( )->dbg_name( ).c_str( ), to_context.dbg_name( ).c_str( ) );
         p_event->send( to_context );

         break;
      }
      case ipc::eCommand::RegisterServer:
      {
         service::Passport server_passport;
         if( false == package.data( server_passport ) )
         {
            SYS_ERR( "parce package error" );
            return false;
         }
         SYS_INF( "register server: %s", server_passport.dbg_name( ).c_str( ) );

         const auto& pid = channel::recv::pid( p_socket );
         const auto& pid_test = server_passport.address.context( ).pid( );
         if( pid != pid_test )
         {
            SYS_ERR( "PID mismatch %s != %s", pid.dbg_name( ).c_str( ), pid_test.dbg_name( ).c_str( ) );
            return false;
         }

         if( false == channel::established( pid ) )
         {
            SYS_WRN( "connection was not established to process %s", pid.dbg_name( ).c_str( ) );
            return false;
         }

         interface::server::add( pid, server_passport );
         application::Process::instance( )->service_registry( ).register_server( server_passport );

         break;
      }
      case ipc::eCommand::RegisterClient:
      {
         service::Passport client_passport;
         if( false == package.data( client_passport ) )
         {
            SYS_ERR( "parce package error" );
            return false;
         }
         SYS_INF( "register client: %s", client_passport.dbg_name( ).c_str( ) );

         const auto& pid = channel::recv::pid( p_socket );
         const auto& pid_test = client_passport.address.context( ).pid( );
         if( pid != pid_test )
         {
            SYS_ERR( "PID mismatch %s != %s", pid.dbg_name( ).c_str( ), pid_test.dbg_name( ).c_str( ) );
            return false;
         }

         if( false == channel::established( pid ) )
         {
            SYS_WRN( "connection was not established to process %s", pid.dbg_name( ).c_str( ) );
            return false;
         }

         const auto& server_address = application::Process::instance( )->service_registry( ).server( client_passport );
         if( false == server_address.is_valid( ) )
         {
            SYS_WRN( "server '%s' is not registered", client_passport.dbg_name( ).c_str( ) );
            return false;
         }
         service::Passport server_passport( client_passport.signature, server_address );
         auto p_socket_send = channel::send::socket( pid );
         ipc::Packet packet( ipc::eCommand::RegisterServer, server_passport );
         m_parent.send( packet, p_socket_send );

         interface::client::add( pid, client_passport );
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



SendReceive::Connections::tProcessSocketMap SendReceive::Connections::data::ms_send = { };
SendReceive::Connections::tSocketProcessMap SendReceive::Connections::data::ms_recv = { };
SendReceive::Connections::tProcessServiceMap SendReceive::Connections::data::ms_pending_servers = { };
SendReceive::Connections::tProcessServiceMap SendReceive::Connections::data::ms_servers = { };
SendReceive::Connections::tProcessServiceMap SendReceive::Connections::data::ms_clients = { };

carpc::os::Socket::tSptr SendReceive::Connections::channel::send::create(
        const application::process::ID& pid
      , const ipc::SocketCongiguration& inet_address
   )
{
   auto iterator = data::ms_send.find( pid );
   if( data::ms_send.end( ) != iterator && nullptr != iterator->second )
   {
      SYS_WRN( "send socket already exists for PID %s", pid.dbg_name( ).c_str( ) );
      return iterator->second;
   }

   os::Socket::tSptr p_socket = os::Socket::create_shared( inet_address, configuration::current( ).ipc_app.buffer_size );
   if( os::Socket::eResult::ERROR == p_socket->create( ) )
      return nullptr;
   if( os::Socket::eResult::ERROR == p_socket->connect( ) )
      return nullptr;
   p_socket->unblock( );
   p_socket->info( "connection to process created" );

   if( data::ms_send.end( ) != iterator )
   {
      SYS_WRN( "send socket does not exist for PID %s", pid.dbg_name( ).c_str( ) );
      iterator->second = p_socket;
   }
   else
   {
      SYS_WRN( "PID %s was registered and socket created", pid.dbg_name( ).c_str( ) );
      data::ms_send.emplace( pid, p_socket );
   }

   return p_socket;
}

bool SendReceive::Connections::channel::send::remove( const application::process::ID& pid )
{
   return 0 != data::ms_send.erase( pid );
}

carpc::os::Socket::tSptr SendReceive::Connections::channel::send::socket( const application::process::ID& pid )
{
   auto iterator = data::ms_send.find( pid );
   if( data::ms_send.end( ) == iterator )
      return nullptr;

   return iterator->second;
}

const carpc::application::process::ID& SendReceive::Connections::channel::send::pid( os::Socket::tSptr p_socket )
{
   auto iterator = std::find_if(
           data::ms_send.begin( )
         , data::ms_send.end( )
         , [ p_socket ]( auto pair ){ return pair.second == p_socket; }
      );
   if( data::ms_send.end( ) == iterator )
      return application::process::ID::invalid;

   return iterator->first;
}

SendReceive::Connections::tProcessSocketMap& SendReceive::Connections::channel::send::collection( )
{
   return data::ms_send;
}

bool SendReceive::Connections::channel::recv::add( os::Socket::tSptr p_socket )
{
   return data::ms_recv.emplace( p_socket, application::process::ID::invalid ).second;
}

bool SendReceive::Connections::channel::recv::update( os::Socket::tSptr p_socket, const application::process::ID& pid )
{
   auto iterator = data::ms_recv.find( p_socket );
   if( data::ms_recv.end( ) == iterator )
   {
      SYS_WRN( "socket was not added before" );
      return false;
   }

   iterator->second = pid;
   return true;
}

bool SendReceive::Connections::channel::recv::remove( os::Socket::tSptr p_socket )
{
   return 0 != data::ms_recv.erase( p_socket );
}

const carpc::application::process::ID& SendReceive::Connections::channel::recv::pid( os::Socket::tSptr p_socket )
{
   auto iterator = data::ms_recv.find( p_socket );
   if( data::ms_recv.end( ) == iterator )
      return application::process::ID::invalid;

   return iterator->second;
}

carpc::os::Socket::tSptr SendReceive::Connections::channel::recv::socket( const application::process::ID& pid )
{
   auto iterator = std::find_if(
           data::ms_recv.begin( )
         , data::ms_recv.end( )
         , [ pid ]( auto pair ){ return pair.second == pid; }
      );
   if( data::ms_recv.end( ) == iterator )
      return nullptr;

   return iterator->first;
}

bool SendReceive::Connections::channel::established( const application::process::ID& pid )
{
   return nullptr != send::socket( pid ) && nullptr != recv::socket( pid );
}

SendReceive::Connections::tSocketProcessMap& SendReceive::Connections::channel::recv::collection( )
{
   return data::ms_recv;
}
