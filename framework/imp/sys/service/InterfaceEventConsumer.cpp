#include "api/sys/service/IServiceThread.hpp"
#include "imp/sys/service/InterfaceEventConsumer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "InterfaceEventConsumer"


using namespace base;



InterfaceEventConsumer::InterfaceEventConsumer( IServiceThread& service )
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

   SYS_ERR( "serevice name: %s", event.info( ).service_name( ).c_str( ) );

   switch( event.info( ).id( ) )
   {
      case events::interface::eID::ServerConnected:
      {
         break;
      }
      case events::interface::eID::ServerDisconnected:
      {
         break;
      }
      case events::interface::eID::ClientConnected:
      {
         break;
      }
      case events::interface::eID::ClientDisconnected:
      {
         break;
      }
      default:
      {
         break;
      }
   }
}
