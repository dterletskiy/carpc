#include "carpc/oswrappers/linux/kernel.hpp"
#include "carpc/helpers/macros/strings.hpp"
#include "carpc/tools/Tools.hpp"
#include "ConnectionProcessor.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "ConnProc"



namespace local {

   bool send( carpc::os::Socket::tSptr p_socket, carpc::ipc::tStream& stream )
   {
      std::size_t size = 0;
      const void* buffer = nullptr;
      stream.buffer( buffer, size );
      p_socket->send( buffer, size );
      stream.reset( );
      return true;
   }

}



ConnectionProcessor::ConnectionProcessor( const carpc::os::os_linux::socket::configuration& configuration, const size_t buffer_size )
{
   mp_socket = carpc::os::SocketServer::create_shared( configuration, buffer_size, *this );
}

ConnectionProcessor::~ConnectionProcessor( )
{
}

bool ConnectionProcessor::init( )
{
   if( carpc::os::Socket::eResult::ERROR == mp_socket->create( ) )
      return false;
   if( carpc::os::Socket::eResult::ERROR == mp_socket->bind( ) )
      return false;
   mp_socket->unblock( );
   if( carpc::os::Socket::eResult::ERROR == mp_socket->listen( ) )
      return false;
   mp_socket->info( "Connection created" );

   return true;
}

bool ConnectionProcessor::shutdown( )
{
   return true;
}

void ConnectionProcessor::read_slave( carpc::os::Socket::tSptr p_socket )
{
   std::size_t recv_size = 0;
   const void* const p_buffer = p_socket->buffer( recv_size );
   carpc::ipc::tStream stream( p_buffer, recv_size );
   while( 0 < stream.size( ) )
   {
      SYS_INF( "stream size = %zu", stream.size( ) );
      carpc::ipc::Packet packet;
      carpc::ipc::deserialize( stream, packet );
      for( carpc::ipc::Package& package : packet.packages( ) )
      {
         SYS_INF( "processing package: %s", package.c_str( ) );
         switch( package.command( ) )
         {
            case carpc::ipc::eCommand::IpcEvent:
            {
               process_broadcast_event( p_socket, package );
               break;
            }
            case carpc::ipc::eCommand::RegisterServer:
            {
               register_server( p_socket, package );
               break;
            }
            case carpc::ipc::eCommand::UnregisterServer:
            {
               unregister_server( p_socket, package );
               break;
            }
            case carpc::ipc::eCommand::RegisterClient:
            {
               register_client( p_socket, package );
               break;
            }
            case carpc::ipc::eCommand::UnregisterClient:
            {
               unregister_client( p_socket, package );
               break;
            }
            default: break;
         }
      }
   }
}

void ConnectionProcessor::connected( carpc::os::Socket::tSptr p_socket )
{
}

void ConnectionProcessor::disconnected( carpc::os::Socket::tSptr p_socket )
{
   for( auto& pair : m_service_map )
   {
      auto& service_signature = pair.first;
      auto& service_connection = pair.second;

      if( std::nullopt != service_connection.server )
      {
         if( service_connection.server.value( ).socket == p_socket )
         {
            SYS_INF( "unregister server: %s / %s",
               service_signature.dbg_name( ).c_str( ),
               service_connection.server.value( ).service_address.dbg_name( ).c_str( )
            );
            service_connection.server.reset( );
         }
      }

      auto& clients = service_connection.clients;
      for( auto iterator = clients.begin( ); iterator != clients.end( ); ++iterator )
      {
         if( iterator->socket == p_socket )
         {
            SYS_INF( "unregister client: %s / %s",
               service_signature.dbg_name( ).c_str( ),
               iterator->service_address.dbg_name( ).c_str( )
            );
            iterator = clients.erase( iterator );
            if( clients.end( ) == iterator )
               break;
         }
      }
   }
}

void ConnectionProcessor::connection_loop( )
{
   while( true )
   {
      mp_socket->select( );
   }
}

void ConnectionProcessor::process_broadcast_event( carpc::os::Socket::tSptr p_socket, carpc::ipc::Package& package )
{
   carpc::ipc::Packet packet;
   packet.add_package( std::move( package ) );
   carpc::ipc::tStream stream;
   carpc::ipc::serialize( stream, packet );

   local::send( p_socket, stream );
}

void ConnectionProcessor::register_server( carpc::os::Socket::tSptr p_socket, carpc::ipc::Package& package )
{
   carpc::service::Passport service_passport;
   carpc::ipc::SocketCongiguration inet_address;
   if( false == package.data( service_passport, inet_address ) )
   {
      SYS_ERR( "parce package error" );
      return;
   }
   SYS_INF( "service passport: %s / socket: %d / inet address: %s",
               service_passport.dbg_name( ).c_str( ),
               p_socket->socket( ),
               inet_address.dbg_name( ).c_str( )
            );

   // Add service signature and server information to DB
   auto service_iterator = m_service_map.find( service_passport.signature );
   if( m_service_map.end( ) == service_iterator )
   {
      service_iterator = m_service_map.emplace( service_passport.signature, ServiceConnection{ } ).first;
   }
   auto& service_connection = service_iterator->second;

   // Check is server with current service signature already registered
   if( std::nullopt != service_connection.server )
   {
      SYS_ERR( "server already registered: %s(%d)",
            service_passport.signature.dbg_name( ).c_str( ), service_iterator->second.server.value( ).socket->socket( )
         );
      return;
   }

   // Add server information to DB
   service_connection.server = Connection{ p_socket, service_passport.address, inet_address };

   // Check if any client registered with current service signature
   if( true == service_connection.clients.empty( ) )
      return;

   // Send notification packet to clients about registered server with current service signature
   SYS_INF( "notifying clients due to registered server with service signature '%s'", service_passport.dbg_name( ).c_str( ) );
   carpc::ipc::Packet packet;
   packet.add_package( carpc::ipc::eCommand::DetectedServer, service_passport, inet_address );
   carpc::ipc::tStream stream;
   carpc::ipc::serialize( stream, packet );
   for( auto connection : service_connection.clients )
      local::send( connection.socket, stream );
}

void ConnectionProcessor::unregister_server( carpc::os::Socket::tSptr p_socket, carpc::ipc::Package& package )
{
   carpc::service::Passport service_passport;
   carpc::ipc::SocketCongiguration inet_address;
   if( false == package.data( service_passport, inet_address ) )
   {
      SYS_ERR( "parce package error" );
      return;
   }
   SYS_INF( "service passport: %s / socket: %d / inet address: %s",
               service_passport.dbg_name( ).c_str( ),
               p_socket->socket( ),
               inet_address.dbg_name( ).c_str( )
            );

   // Find registered server with current service signature 
   auto service_iterator = m_service_map.find( service_passport.signature );
   if( m_service_map.end( ) == service_iterator )
   {
      SYS_ERR( "service signature was not found" );
      return;
   }
   auto& service_connection = service_iterator->second;

   // Check is server with current service signature already registered
   if( std::nullopt == service_connection.server )
   {
      SYS_ERR( "server was not registered: %s(%d)", service_passport.signature.dbg_name( ).c_str( ), p_socket->socket( ) );
      return;
   }

   // Remove service signature and server information from DB
   service_connection.server.reset( );

   // Check if any client registered with current service signature
   if( true == service_connection.clients.empty( ) )
      return;

   // Send notification event to clients about registered server with current service signature
   SYS_INF( "notifying clients due to unregistered server with service signature '%s'", service_passport.dbg_name( ).c_str( ) );
}

void ConnectionProcessor::register_client( carpc::os::Socket::tSptr p_socket, carpc::ipc::Package& package )
{
   carpc::service::Passport service_passport;
   carpc::ipc::SocketCongiguration inet_address;
   if( false == package.data( service_passport, inet_address ) )
   {
      SYS_ERR( "parce package error" );
      return;
   }
   SYS_INF( "service passport: %s / socket: %d / inet address: %s",
               service_passport.dbg_name( ).c_str( ),
               p_socket->socket( ),
               inet_address.dbg_name( ).c_str( )
            );

   // Add service signature to DB if it is not exists
   auto service_iterator = m_service_map.find( service_passport.signature );
   if( m_service_map.end( ) == service_iterator )
   {
      service_iterator = m_service_map.emplace( service_passport.signature, ServiceConnection{ } ).first;
   }
   auto& service_connection = service_iterator->second;

   // Add client information to DB
   auto result = service_connection.clients.emplace( Connection{ p_socket, service_passport.address, inet_address } );
   if( false == result.second )
   {
      if( service_connection.clients.end( ) != result.first )
      {
         SYS_ERR( "client already registered %s(%d)", service_passport.signature.dbg_name( ).c_str( ), result.first->socket->socket( ) );
      }
      else
      {
         SYS_ERR( "unable register client %s", service_passport.signature.dbg_name( ).c_str( ) );
      }
      return;
   }

   // Check if any server already registered with current service signature
   if( std::nullopt == service_connection.server )
      return;

   // Send notification event to server about registered client with current service signature

   // Send notification events to client about registered server with current service signature
   const Connection& server = service_connection.server.value( );
   SYS_INF( "notifying server due to registered client with service signature '%s'", service_passport.dbg_name( ).c_str( ) );
   carpc::ipc::Packet packet;
   packet.add_package(
      carpc::ipc::eCommand::DetectedServer,
      carpc::service::Passport( service_passport.signature, server.service_address ),
      server.inet_address
   );
   carpc::ipc::tStream stream;
   carpc::ipc::serialize( stream, packet );
   local::send( p_socket, stream );
}

void ConnectionProcessor::unregister_client( carpc::os::Socket::tSptr p_socket, carpc::ipc::Package& package )
{
   carpc::service::Passport service_passport;
   carpc::ipc::SocketCongiguration inet_address;
   if( false == package.data( service_passport, inet_address ) )
   {
      SYS_ERR( "parce package error" );
      return;
   }
   SYS_INF( "service passport: %s / socket: %d / inet address: %s",
               service_passport.dbg_name( ).c_str( ),
               p_socket->socket( ),
               inet_address.dbg_name( ).c_str( )
            );

   // Find registered client with current service signature 
   auto service_iterator = m_service_map.find( service_passport.signature );
   if( m_service_map.end( ) == service_iterator )
   {
      SYS_WRN( "service signature was not found" );
      return;
   }
   auto& service_connection = service_iterator->second;

   // Find registered client with current service signature and remove it if exists
   const size_t result = service_connection.clients.erase( Connection{ p_socket, service_passport.address, inet_address } );
   // Check if any client was removed
   if( 0 == result )
   {
      SYS_ERR( "client was not registered: %s(%d)", service_passport.signature.dbg_name( ).c_str( ), p_socket->socket( ) );
      return;
   }

   // Check if server registered with current service signature
   if( std::nullopt == service_connection.server )
      return;

   // Send notification event about unregistered client with current service signature
   SYS_INF( "notifying server due to unregistered client with service signature '%s'", service_passport.dbg_name( ).c_str( ) );
}
