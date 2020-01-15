#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/service/Service.hpp"
#include "imp/sys/events/SysEvent.hpp"
#include "imp/sys/service/ServiceEventConsumer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "ServiceEventConsumer"


namespace base {



ServiceEventConsumer::ServiceEventConsumer( ServicePtr p_service )
   : mp_service( p_service )
{
   ServiceEvent::Event::set_notification( true, this );
}

ServiceEventConsumer::~ServiceEventConsumer( )
{
   ServiceEvent::Event::set_notification( false, this );
}

void ServiceEventConsumer::process_event( const ServiceEvent::Event& event )
{
   SYS_INF( "command = %#zx, info = %s", static_cast< size_t >( event.data( )->command ), event.data( )->info.c_str( ) );

   ServicePtr p_service = mp_service.lock();
   if( nullptr == p_service )
      return;

   switch( event.data( )->command )
   {
      case eServiceCommand::boot:
      {
         SysEvent::Event::send_event( { eSysCommand::boot, "request boot" }, eCommType::ETC );
         break;
      }
      case eServiceCommand::shutdown:
      {
         p_service->stop( );
         break;
      }
      case eServiceCommand::ping:
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
