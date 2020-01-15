#include "api/sys/comm/Event.hpp"
#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/service/Service.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/service/ServiceBrocker.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "SrvBr"



namespace base {



ServiceBrockerPtr ServiceBrocker::mp_instance;

ServiceBrocker::ServiceBrocker( )
   : m_events( )
{
   mp_thread = std::make_shared< base::os::Thread >( std::bind( &ServiceBrocker::thread_loop, this ) );

   SYS_INF( "Created" );
}

ServiceBrocker::~ServiceBrocker( )
{
   SYS_INF( "Destroyed" );
}

ServiceBrockerPtr ServiceBrocker::instance( )
{
   os::Mutex mutex( true );
   if( !mp_instance )
   {
      mp_instance.reset( new ServiceBrocker( ) );
   }

   return mp_instance;
}

void ServiceBrocker::thread_loop( )
{
   SYS_INF( "enter" );
   m_started = true;

   while( started( ) )
   {
      EventPtr p_event = get_event( );
      SYS_INF( "processing event (%s)", p_event->type_id( ).c_str( ) );
      notify( p_event );
   }

   SYS_INF( "exit" );
}

bool ServiceBrocker::start( )
{
   SYS_INF( "Starting ServiceBrocker" );
   bool result = mp_thread->run( );
   if( false == result )
   {
      SYS_ERR( "ServiceBrocker can't be started" );
   }

   return result;
}

void ServiceBrocker::stop( )
{
   SYS_INF( "Stopping ServiceBrocker" );
   m_started = false;
}

bool ServiceBrocker::insert_event( const EventPtr p_event )
{
   if( false == m_started )
   {
      SYS_WRN( "ServiceBrocker is not started" );
      return false;
   }
   SYS_INF( "inserting event (%s)", p_event->type_id( ).c_str( ) );

   m_buffer_cond_var.lock( );
   m_events.push_back( p_event );
   m_buffer_cond_var.notify( );
   m_buffer_cond_var.unlock( );
   return true;
}

EventPtr ServiceBrocker::get_event( )
{
   m_buffer_cond_var.lock( );
   if( true == m_events.empty( ) )
   {
      SYS_INF( "ServiceBrocker is waiting for event..." );
      m_buffer_cond_var.wait( );
   }
   EventPtr p_event = m_events.front( );
   m_events.pop_front( );
   SYS_INF( "received event (%s)", p_event->type_id( ).c_str( ) );
   m_buffer_cond_var.unlock( );

   return p_event;
}

void ServiceBrocker::notify( const EventPtr p_event )
{
   ServicePtrList service_list = base::ServiceProcess::instance( )->service_list( );
   for( auto p_service : service_list )
   {
      if( true == p_service->is_subscribed( p_event->type_id( ) ) )
         p_service->insert_event( p_event );
   }
}



} // namespace base
