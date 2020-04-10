#include "api/sys/oswrappers/Mutex.hpp"
#include "imp/sys/events/SysEvent.hpp"
#include "imp/sys/service/ServiceEventConsumer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "ServiceEventConsumer"


namespace base {



ServiceEventConsumer::ServiceEventConsumer( IServiceThread::tSptr p_service )
   : mp_service( p_service )
{
   ServiceEvent::Event::set_notification( this );
}

ServiceEventConsumer::~ServiceEventConsumer( )
{
   ServiceEvent::Event::clear_notification( this );
}

void ServiceEventConsumer::process_event( const ServiceEvent::Event& event )
{
   SYS_INF( "command = %#zx, info = %s", static_cast< size_t >( event.data( )->command ), event.data( )->info.c_str( ) );

   IServiceThread::tSptr p_service = mp_service.lock();
   if( nullptr == p_service )
      return;

   switch( event.data( )->command )
   {
      case eServiceCommand::boot:
      {
         SysEvent::Event::create_send( { eSysCommand::boot, "request boot" }, eCommType::ETC );
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
