#include "oswrappers/Thread.hpp"
#include "service/ServiceProcess.hpp"
#include "service/ServiceBrockerDSI.hpp"
#include "service/ServiceBrocker.hpp"
#include "service/Service.hpp"
#include "Event.hpp"

#include "Trace.hpp"
#define CLASS_ABBR "Event"


namespace base {



/*************************
 *
 * Event_ID implementation
 *
 ************************/
ByteBuffer& operator << ( ByteBuffer& buffer, const Event_ID& event_id )
{
   buffer << event_id.m_id;
   return buffer;
}

ByteBuffer& operator >> ( ByteBuffer& buffer, Event_ID& event_id )
{
   buffer >> event_id.m_id;
   return buffer;
}

Event_ID::Event_ID( const std::string& id )
   : m_id( id )
{
}

Event_ID::~Event_ID( )
{
}

const char* Event_ID::c_str( ) const
{
   return m_id.c_str( );
}

const bool Event_ID::operator==( const Event_ID& id ) const
{
   return m_id == id.m_id;
}

const bool Event_ID::operator!=( const Event_ID& id ) const
{
   return m_id != id.m_id;
}

const bool Event_ID::operator>( const Event_ID& id ) const 
{
   return m_id > id.m_id;
}

const bool Event_ID::operator<( const Event_ID& id ) const 
{
   return m_id < id.m_id;
}




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
   // SYS_INF( "event id: %#x / consumer: %p / is_set: %s", type_id, p_consumer, is_set ? "true" : "false" );
   IServicePtr p_service = ServiceProcess::instance( )->service( os::Thread::current_id( ) );
   if( InvalidServicePtr == p_service )
      return false;

   // SYS_INF( "event id: %#x / service: %s", type_id, p_service->name( ).c_str( ) );
   if( true == is_set )
   {
      p_service->set_notification( type_id, p_consumer);
   }
   else
   {
      p_service->clear_notification( type_id, p_consumer);
   }

   return true;
}

bool Event::send( EventPtr p_event, const Event_ID& type_id )
{
   switch( p_event->m_comm_type )
   {
      case eCommType::ETC:
      {
         IServicePtr p_service = ServiceProcess::instance( )->service( os::Thread::current_id( ) );
         if( InvalidServicePtr == p_service )
            return false;

         return p_service->insert_event( p_event );
      }
      case eCommType::ITC:
      {
         IServiceBrockerPtr p_service_brocker = ServiceProcess::instance( )->service_brocker( );
         if( InvalidServiceBrockerPtr == p_service_brocker )
            return false;

         return p_service_brocker->insert_event( p_event );
      }
      case eCommType::IPC:
      {
         IServiceBrockerDsiPtr p_service_brocker_dsi = ServiceProcess::instance( )->service_brocker_dsi( );
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
