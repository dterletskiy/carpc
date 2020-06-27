#include "api/sys/dsi/Types.hpp"
#include "api/sys/service/ServiceIpcThread.hpp"
#include "imp/sys/service/InterfaceEventConsumer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "InterfaceEventConsumer"


using namespace base;



InterfaceEventConsumer::InterfaceEventConsumer( ServiceIpcThread& service )
   : m_service( service )
{
   events::interface::Interface::Event::set_notification( this, { InvalidServiceName, events::interface::eID::ServerConnected } );
   events::interface::Interface::Event::set_notification( this, { InvalidServiceName, events::interface::eID::ServerDisconnected } );
   events::interface::Interface::Event::set_notification( this, { InvalidServiceName, events::interface::eID::ClientConnected } );
   events::interface::Interface::Event::set_notification( this, { InvalidServiceName, events::interface::eID::ClientDisconnected } );
}

InterfaceEventConsumer::~InterfaceEventConsumer( )
{
   events::interface::Interface::Event::clear_all_notifications( this );
}

void InterfaceEventConsumer::process_event( const events::interface::Interface::Event& event )
{
   SYS_INF( "id = %s", events::interface::c_str( event.info( ).id( ) ) );

   const std::string& service_name = event.info( ).service_name( );
   SYS_ERR( "serevice name: %s", service_name.c_str( ) );

   dsi::eCommand command = dsi::eCommand::Undefined;
   switch( event.info( ).id( ) )
   {
      case events::interface::eID::ServerConnected:
      {
         command = dsi::eCommand::RegisterServer;
         break;
      }
      case events::interface::eID::ServerDisconnected:
      {
         command = dsi::eCommand::UnregisterServer;
         break;
      }
      case events::interface::eID::ClientConnected:
      {
         command = dsi::eCommand::RegisterClient;
         break;
      }
      case events::interface::eID::ClientDisconnected:
      {
         command = dsi::eCommand::UnregisterClient;
         break;
      }
      default:
      {
         break;
      }
   }
}
