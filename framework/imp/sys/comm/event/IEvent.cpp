#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/service/Service.hpp"
#include "api/sys/service/ServiceBrocker.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/comm/event/IEventSignature.hpp"
#include "api/sys/comm/event/IEvent.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IEvent"


namespace base {



const bool IEvent::set_notification( IEventConsumer* p_consumer, const IEventSignature& signature )
{
   SYS_INF( "event: %s / consumer: %p", signature.name( ).c_str( ), p_consumer );
   ServicePtr p_service = ServiceProcess::instance( )->current_service( );
   if( InvalidServicePtr == p_service )
      return false;

   SYS_INF( "event: %s / service: %s", signature.name( ).c_str( ), p_service->name( ).c_str( ) );
   p_service->set_notification( signature, p_consumer );

   return true;
}

const bool IEvent::clear_notification( IEventConsumer* p_consumer, const IEventSignature& signature )
{
   SYS_INF( "event: %s / consumer: %p", signature.name( ).c_str( ), p_consumer );
   ServicePtr p_service = ServiceProcess::instance( )->current_service( );
   if( InvalidServicePtr == p_service )
      return false;

   SYS_INF( "event: %s / service: %s", signature.name( ).c_str( ), p_service->name( ).c_str( ) );
   p_service->clear_notification( signature, p_consumer );

   return true;
}

const bool IEvent::clear_all_notifications( IEventConsumer* p_consumer, const IEventSignature& signature )
{
   SYS_INF( "event: %s / consumer: %p", signature.name( ).c_str( ), p_consumer );
   ServicePtr p_service = ServiceProcess::instance( )->current_service( );
   if( InvalidServicePtr == p_service )
      return false;

   SYS_INF( "event: %s / service: %s", signature.name( ).c_str( ), p_service->name( ).c_str( ) );
   p_service->clear_all_notifications( signature, p_consumer );

   return true;
}

const bool IEvent::send( EventPtr p_event, const eCommType comm_type )
{
   const eCommType type = comm_type == eCommType::NONE ? p_event->comm_type( ) : comm_type;
   switch( type )
   {
      case eCommType::ETC:
      {
         ServicePtr p_service = ServiceProcess::instance( )->current_service( );
         if( InvalidServicePtr == p_service )
         {
            SYS_ERR( "sending ETC event not from service thread" );
            return false;
         }

         return p_service->insert_event( p_event );
      }
      case eCommType::ITC:
      {
         bool result = true;
         ServicePtrList service_list = base::ServiceProcess::instance( )->service_list( );
         for( auto p_service : service_list )
            result &= p_service->insert_event( p_event );
         return result;
      }
      case eCommType::IPC:
      {
         ServiceBrockerPtr p_service_brocker = ServiceProcess::instance( )->service_brocker( );
         if( InvalidServiceBrockerPtr == p_service_brocker )
         {
            SYS_ERR( "ServiceBrocker is not started" );
            return false;
         }

         return p_service_brocker->insert_event( p_event );
      }
      default: break;
   }

   return true;
}

const bool IEvent::send_to_context( EventPtr p_event, ServicePtrW pw_service  )
{
   ServicePtr p_service = pw_service.lock( );
   if( InvalidServicePtr == p_service )
   {
      SYS_ERR( "sending ETC event to not valid service thread" );
      return false;
   }

   return p_service->insert_event( p_event );
}



} // namespace base
