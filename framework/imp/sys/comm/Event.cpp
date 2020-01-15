#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/service/ServiceBrockerDSI.hpp"
#include "api/sys/service/ServiceBrocker.hpp"
#include "api/sys/service/Service.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/comm/Event.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Event"


namespace base {



/*************************
 *
 * Event implementation
 *
 ************************/
Event::Event( const eCommType comm_type )
   : m_comm_type( comm_type )
{
}

Event::Event( const Event& event )
   : m_comm_type( event.m_comm_type )
{
}

Event::~Event( )
{
}

bool Event::set_notification( bool is_set, IEventConsumer* p_consumer, const Event_ID& type_id )
{
   SYS_INF( "event id: %s / consumer: %p / is_set: %s", type_id.c_str( ), p_consumer, BOOL_TO_STRING( is_set ) );
   ServicePtr p_service = ServiceProcess::instance( )->service( os::Thread::current_id( ) );
   if( InvalidServicePtr == p_service )
      return false;

   SYS_INF( "event id: %s / service: %s", type_id.c_str( ), p_service->name( ).c_str( ) );
   if( true == is_set )
   {
      p_service->set_notification( type_id, p_consumer );
   }
   else
   {
      p_service->clear_notification( type_id, p_consumer );
   }

   return true;
}

bool Event::send( EventPtr p_event, const eCommType comm_type )
{
   const eCommType type = comm_type == eCommType::NONE ? p_event->comm_type( ) : comm_type;
   switch( type )
   {
      case eCommType::ETC:
      {
         ServicePtr p_service = ServiceProcess::instance( )->service( os::Thread::current_id( ) );
         if( InvalidServicePtr == p_service )
            return false;

         return p_service->insert_event( p_event );
      }
      case eCommType::ITC:
      {
         ServiceBrockerPtr p_service_brocker = ServiceProcess::instance( )->service_brocker( );
         if( InvalidServiceBrockerPtr == p_service_brocker )
            return false;

         return p_service_brocker->insert_event( p_event );
      }
      case eCommType::IPC:
      {
         ServiceBrockerDsiPtr p_service_brocker_dsi = ServiceProcess::instance( )->service_brocker_dsi( );
         if( InvalidServiceBrockerDsiPtr == p_service_brocker_dsi )
            return false;

         return p_service_brocker_dsi->insert_event( p_event );
      }
      default:
      {
         break;
      }
   }

   return true;
}

const eCommType& Event::comm_type( ) const
{
   return m_comm_type;
}



} // namespace base
