#include "api/sys/service/IServiceThread.hpp"
#include "imp/sys/service/ServiceEventConsumer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "ServiceEventConsumer"


using namespace base;



ServiceEventConsumer::ServiceEventConsumer( IServiceThread& service )
   : m_service( service )
{
   events::service::Service::Event::set_notification( this, { events::service::eID::boot } );
   events::service::Service::Event::set_notification( this, { events::service::eID::shutdown } );
   events::service::Service::Event::set_notification( this, { events::service::eID::ping } );
}

ServiceEventConsumer::~ServiceEventConsumer( )
{
   events::service::Service::Event::clear_all_notifications( this );
}

void ServiceEventConsumer::process_event( const events::service::Service::Event& event )
{
   SYS_INF( "id = %s", events::service::c_str( event.info( ).id( ) ) );

   std::string message = "";
   if( event.data( ) ) message = event.data( )->message;

   switch( event.info( ).id( ) )
   {
      case events::service::eID::boot:
      {
         m_service.boot( message );
         break;
      }
      case events::service::eID::shutdown:
      {
         m_service.shutdown( message );
         break;
      }
      case events::service::eID::ping:
      {
         break;
      }
      default:
      {
         break;
      }
   }
}
