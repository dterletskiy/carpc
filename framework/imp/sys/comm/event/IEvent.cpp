#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/service/ServiceIpcThread.hpp"
#include "api/sys/service/ServiceThread.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/comm/event/IEvent.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IEvent"


namespace base {




const bool IEvent::set_notification( IAsync::IConsumer* p_consumer, const ISignature& signature )
{
   IServiceThread::tSptr p_service = ServiceProcess::instance( )->current_service( );
   if( nullptr == p_service )
   {
      SYS_ERR( "subscribe on event not from service thread" );
      return false;
   }

   SYS_INF( "event: %s / consumer: %p / service: %s", signature.name( ).c_str( ), p_consumer, p_service->name( ).c_str( ) );
   p_service->set_notification( signature, p_consumer );

   return true;
}

const bool IEvent::clear_notification( IAsync::IConsumer* p_consumer, const ISignature& signature )
{
   IServiceThread::tSptr p_service = ServiceProcess::instance( )->current_service( );
   if( nullptr == p_service )
   {
      SYS_ERR( "unsubscribe from event not from service thread" );
      return false;
   }

   SYS_INF( "event: %s / consumer: %p / service: %s", signature.name( ).c_str( ), p_consumer, p_service->name( ).c_str( ) );
   p_service->clear_notification( signature, p_consumer );

   return true;
}

const bool IEvent::clear_all_notifications( IAsync::IConsumer* p_consumer, const ISignature& signature )
{
   IServiceThread::tSptr p_service = ServiceProcess::instance( )->current_service( );
   if( nullptr == p_service )
   {
      SYS_ERR( "unsubscribe from event not from service thread" );
      return false;
   }

   SYS_INF( "event: %s / consumer: %p / service: %s", signature.name( ).c_str( ), p_consumer, p_service->name( ).c_str( ) );
   p_service->clear_all_notifications( signature, p_consumer );

   return true;
}

const bool IEvent::send( tSptr p_event, const eCommType comm_type )
{
   if( !p_event ) return false;

   const eCommType type = comm_type == eCommType::NONE ? p_event->comm_type( ) : comm_type;
   switch( type )
   {
      case eCommType::ETC:
      {
         IServiceThread::tSptr p_service = ServiceProcess::instance( )->current_service( );
         if( nullptr == p_service )
         {
            SYS_ERR( "sending ETC event not from service thread" );
            return false;
         }

         return p_service->insert_event( p_event );
      }
      case eCommType::ITC:
      {
         bool result = true;
         IServiceThread::tSptrList service_list = base::ServiceProcess::instance( )->service_list( );
         for( auto p_service : service_list )
            result &= p_service->insert_event( p_event );
         return result;
      }
      case eCommType::IPC:
      {
         IServiceThread::tSptr p_service_ipc = ServiceProcess::instance( )->service_ipc( );
         if( nullptr == p_service_ipc )
         {
            SYS_ERR( "ServiceIpcThread is not started" );
            return false;
         }

         return p_service_ipc->insert_event( p_event );
      }
      default: break;
   }

   return true;
}

const bool IEvent::send_to_context( tSptr p_event, IServiceThread::tWptr pw_service )
{
   if( !p_event ) return false;

   IServiceThread::tSptr p_service = pw_service.lock( );
   if( nullptr == p_service )
   {
      SYS_ERR( "sending ETC event to not valid service thread" );
      return false;
   }

   return p_service->insert_event( p_event );
}

void IEvent::process( IAsync::IConsumer* p_consumer ) const
{
   SYS_INF( "consumer: %p", p_consumer );
   if( nullptr != p_consumer ) process_event( p_consumer );
}



} // namespace base
