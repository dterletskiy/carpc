#include "api/sys/oswrappers/Mutex.hpp"
#include "imp/sys/service/ServiceEventConsumer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "ServiceEventConsumer"


namespace base {



ServiceEventConsumer::ServiceEventConsumer( IServiceThread::tSptr p_service )
   : mp_service( p_service )
{
   events::service::ServiceEvent::Event::set_notification( this, { events::service::eID::boot } );
   events::service::ServiceEvent::Event::set_notification( this, { events::service::eID::shutdown } );
   events::service::ServiceEvent::Event::set_notification( this, { events::service::eID::ping } );
}

ServiceEventConsumer::~ServiceEventConsumer( )
{
   events::service::ServiceEvent::Event::clear_all_notifications( this );
}

void ServiceEventConsumer::process_event( const events::service::ServiceEvent::Event& event )
{
   std::string message = "";
   if( event.data( ) ) message = event.data( )->message;

   SYS_INF( "message = %s", message.c_str( ) );

   IServiceThread::tSptr p_service = mp_service.lock();
   if( nullptr == p_service )
      return;

   switch( event.info( ).id( ) )
   {
      case events::service::eID::boot:
      {
         p_service->boot( message );
         break;
      }
      case events::service::eID::shutdown:
      {
         p_service->shutdown( message );
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



} // namespace base
