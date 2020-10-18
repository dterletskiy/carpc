#include "api/sys/oswrappers/linux/kernel.hpp"
#include "api/sys/helpers/macros/strings.hpp"
#include "api/sys/tools/Tools.hpp"
#include "ConnectionProcessor.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "ConnProc"



namespace local {

   bool send( base::os::Socket::tSptr p_socket, base::dsi::tByteStream& stream )
   {
      size_t size = 0;
      const void* buffer = nullptr;
      if( false == stream.is_linear( buffer, size ) )
      {
         SYS_WRN( "stream is not linear" );
         return false;
      }
      p_socket->send( buffer, size );
      stream.erase( size );
      return true;
   }

}



ConnectionProcessor::ConnectionProcessor( const base::os::linux::socket::configuration& configuration, const size_t buffer_size )
   : base::os::SocketServer( configuration, buffer_size )
{
}

ConnectionProcessor::~ConnectionProcessor( )
{
}

bool ConnectionProcessor::init( )
{
   if( eResult::ERROR == create( ) )
      return false;
   if( eResult::ERROR == bind( ) )
      return false;
   unblock( );
   if( eResult::ERROR == listen( ) )
      return false;
   info( "Connection created" );

   return true;
}

bool ConnectionProcessor::shutdown( )
{
   return true;
}

void ConnectionProcessor::read_slave( base::os::Socket::tSptr p_socket )
{
   size_t recv_size = 0;
   const void* const p_buffer = p_socket->buffer( recv_size );
   // p_socket->send( p_buffer, recv_size );

   base::dsi::tByteStream stream;
   stream.push( p_buffer, recv_size );
   while( 0 < stream.size( ) )
   {
      SYS_INF( "stream size = %zu", stream.size( ) );
      base::dsi::Packet packet;
      stream.pop( packet );
      for( base::dsi::Package& package : packet.packages( ) )
      {
         SYS_INF( "processing package: %s", package.c_str( ) )
         switch( package.command( ) )
         {
            case base::dsi::eCommand::BroadcastEvent:
            {
               process_broadcast_event( p_socket, package );
               break;
            }
            case base::dsi::eCommand::RegisterServer:
            {
               register_server( p_socket, package );
               break;
            }
            case base::dsi::eCommand::UnregisterServer:
            {
               unregister_server( p_socket, package );
               break;
            }
            case base::dsi::eCommand::RegisterClient:
            {
               register_client( p_socket, package );
               break;
            }
            case base::dsi::eCommand::UnregisterClient:
            {
               unregister_client( p_socket, package );
               break;
            }
            default: break;
         }
      }
   }
}

void ConnectionProcessor::connection_loop( )
{
   while( true )
   {
      select( );
   }
}

void ConnectionProcessor::process_broadcast_event( base::os::Socket::tSptr p_socket, base::dsi::Package& package )
{
   base::dsi::Packet packet;
   packet.add_package( std::move( package ) );
   base::dsi::tByteStream stream;
   stream.push( packet );

   local::send( p_socket, stream );
}

void ConnectionProcessor::register_server( base::os::Socket::tSptr p_socket, base::dsi::Package& package )
{
   base::service::Signature service_signature;
   base::service::Address service_address;
   base::dsi::SocketCongiguration inet_address;
   if( false == package.data( service_signature, service_address, inet_address ) )
   {
      SYS_ERR( "parce package error" );
      return;
   }
   SYS_INF( "service signature: %s / service address: %s / socket: %d / inet address: %s",
               service_signature.name( ).c_str( ),
               service_address.name( ).c_str( ),
               p_socket->socket( ),
               inet_address.name( ).c_str( )
            );

   // Add service signature and server information to DB
   auto service_iterator = m_service_map.find( service_signature );
   if( m_service_map.end( ) == service_iterator )
   {
      service_iterator = m_service_map.emplace( service_signature, ServiceConnection{ } ).first;
   }
   auto& service_connection = service_iterator->second;

   // Check is server with current service signature already registered
   if( std::nullopt != service_connection.server )
   {
      SYS_ERR( "server already registered: %s(%d)",
            service_signature.name( ).c_str( ), service_iterator->second.server.value( ).socket->socket( )
         );
      return;
   }

   // Add server information to DB
   service_connection.server = Connection{ p_socket, service_address, inet_address };

   // Check if any client registered with current service signature
   if( true == service_connection.clients.empty( ) )
      return;

   // Send notification packet to clients about registered server with current service signature
   SYS_INF( "notifying clients due to registered server with service signature '%s'", service_signature.name( ).c_str( ) );
   base::dsi::Packet packet;
   packet.add_package( base::dsi::eCommand::DetectedServer, service_signature, service_address, inet_address );
   base::dsi::tByteStream stream;
   stream.push( packet );
   for( auto connection : service_connection.clients )
      local::send( connection.socket, stream );
}

void ConnectionProcessor::unregister_server( base::os::Socket::tSptr p_socket, base::dsi::Package& package )
{
   base::service::Signature service_signature;
   base::service::Address service_address;
   base::dsi::SocketCongiguration inet_address;
   if( false == package.data( service_signature, service_address, inet_address ) )
   {
      SYS_ERR( "parce package error" );
      return;
   }
   SYS_INF( "service signature: %s / service address: %s / socket: %d / inet address: %s",
               service_signature.name( ).c_str( ),
               service_address.name( ).c_str( ),
               p_socket->socket( ),
               inet_address.name( ).c_str( )
            );

   // Find registered server with current service signature 
   auto service_iterator = m_service_map.find( service_signature );
   if( m_service_map.end( ) == service_iterator )
   {
      SYS_ERR( "service signature was not found" );
      return;
   }
   auto& service_connection = service_iterator->second;

   // Check is server with current service signature already registered
   if( std::nullopt == service_connection.server )
   {
      SYS_ERR( "server was not registered: %s(%d)", service_signature.name( ).c_str( ), p_socket->socket( ) );
      return;
   }

   // Remove service signature and server information from DB
   service_connection.server.reset( );

   // Check if any client registered with current service signature
   if( true == service_connection.clients.empty( ) )
      return;

   // Send notification event to clients about registered server with current service signature
   SYS_INF( "notifying clients due to unregistered server with service signature '%s'", service_signature.name( ).c_str( ) );
}

void ConnectionProcessor::register_client( base::os::Socket::tSptr p_socket, base::dsi::Package& package )
{
   base::service::Signature service_signature;
   base::service::Address service_address;
   base::dsi::SocketCongiguration inet_address;
   if( false == package.data( service_signature, service_address, inet_address ) )
   {
      SYS_ERR( "parce package error" );
      return;
   }
   SYS_INF( "service signature: %s / service address: %s / socket: %d / inet address: %s",
               service_signature.name( ).c_str( ),
               service_address.name( ).c_str( ),
               p_socket->socket( ),
               inet_address.name( ).c_str( )
            );

   // Add service signature to DB if it is not exists
   auto service_iterator = m_service_map.find( service_signature );
   if( m_service_map.end( ) == service_iterator )
   {
      service_iterator = m_service_map.emplace( service_signature, ServiceConnection{ } ).first;
   }
   auto& service_connection = service_iterator->second;

   // Add client information to DB
   auto result = service_connection.clients.emplace( Connection{ p_socket, service_address, inet_address } );
   if( false == result.second )
   {
      if( service_connection.clients.end( ) != result.first )
      {
         SYS_ERR( "client already registered %s(%d)", service_signature.name( ).c_str( ), result.first->socket->socket( ) );
      }
      else
      {
         SYS_ERR( "unable register client %s", service_signature.name( ).c_str( ) );
      }
      return;
   }

   // Check if any server already registered with current service signature
   if( std::nullopt == service_connection.server )
      return;

   // Send notification event to server about registered client with current service signature

   // Send notification events to client about registered server with current service signature
   const Connection& server = service_connection.server.value( );
   SYS_INF( "notifying server due to registered client with service signature '%s'", service_signature.name( ).c_str( ) );
   base::dsi::Packet packet;
   packet.add_package( base::dsi::eCommand::DetectedServer, service_signature, server.service_address, server.inet_address );
   base::dsi::tByteStream stream;
   stream.push( packet );
   local::send( p_socket, stream );
}

void ConnectionProcessor::unregister_client( base::os::Socket::tSptr p_socket, base::dsi::Package& package )
{
   base::service::Signature service_signature;
   base::service::Address service_address;
   base::dsi::SocketCongiguration inet_address;
   if( false == package.data( service_signature, service_address, inet_address ) )
   {
      SYS_ERR( "parce package error" );
      return;
   }
   SYS_INF( "service signature: %s / service address: %s / socket: %d / inet address: %s",
               service_signature.name( ).c_str( ),
               service_address.name( ).c_str( ),
               p_socket->socket( ),
               inet_address.name( ).c_str( )
            );

   // Find registered client with current service signature 
   auto service_iterator = m_service_map.find( service_signature );
   if( m_service_map.end( ) == service_iterator )
   {
      SYS_WRN( "service signature was not found" );
      return;
   }
   auto& service_connection = service_iterator->second;

   // Find registered client with current service signature and remove it if exists
   const size_t result = service_connection.clients.erase( Connection{ p_socket, service_address, inet_address } );
   // Check if any client was removed
   if( 0 == result )
   {
      SYS_ERR( "client was not registered: %s(%d)", service_signature.name( ).c_str( ), p_socket->socket( ) );
      return;
   }

   // Check if server registered with current service signature
   if( std::nullopt == service_connection.server )
      return;

   // Send notification event about unregistered client with current service signature
   SYS_INF( "notifying server due to unregistered client with service signature '%s'", service_signature.name( ).c_str( ) );
}
