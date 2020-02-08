#include "api/sys/comm/interface/Server.hpp"
#include "api/sys/comm/interface/Client.hpp"
#include "imp/sys/comm/interface/InterfaceStatusHandler.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "InterfaceStatusHandler"



INIT_EVENT( base::InterfaceRegistryEvent );



namespace base {



InterfaceStatusHandler::InterfaceStatusHandler( )
{
   InterfaceRegistryEvent::Event::set_notification( this, eInterfaceRegistry::ServerConnected );
   InterfaceRegistryEvent::Event::set_notification( this, eInterfaceRegistry::ServerDisconnected );
   InterfaceRegistryEvent::Event::set_notification( this, eInterfaceRegistry::ClientConnected );
   InterfaceRegistryEvent::Event::set_notification( this, eInterfaceRegistry::ClientDisconnected );
}

InterfaceStatusHandler::~InterfaceStatusHandler( )
{
   InterfaceRegistryEvent::Event::clear_all_notifications( this );
}

void InterfaceStatusHandler::process_event( const InterfaceRegistryEvent::Event& event )
{
   const Server* const p_server = event.data( )->p_server;
   const Client* const p_client = event.data( )->p_client;
   SYS_TRC( "server: %p / client: %p", p_server, p_client );
   switch( event.info( ).id( ) )
   {
      case eInterfaceRegistry::ServerConnected:
      {
         p_client->connected( p_server );
         break;
      }
      case eInterfaceRegistry::ServerDisconnected:
      {
         p_client->disconnected( p_server );
         break;
      }
      case eInterfaceRegistry::ClientConnected:
      {
         p_server->connected( p_client );
         break;
      }
      case eInterfaceRegistry::ClientDisconnected:
      {
         p_server->disconnected( p_client );
         break;
      }
      default: break;
   }
}



} // namespace base
