#include "api/sys/comm/async/event/Event.hpp"
#include "api/sys/application/Process.hpp"
#include "api/sys/application/Thread.hpp"
#include "imp/sys/application/SystemEventConsumer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Srv"



using namespace base::application;



Thread::Thread( const Configuration& config )
   : IThread( config.m_name, config.m_wd_timeout )
   , m_thread( std::bind( &Thread::thread_loop, this ) )
   , m_event_queue( Process::instance( )->configuration( ).max_priority, config.m_name )
   , m_consumers_map( config.m_name )
   , m_components( )
   , m_component_creators( config.m_component_creators )
{
   SYS_TRC( "'%s': created", m_name.c_str( ) );
}

Thread::~Thread( )
{
   SYS_TRC( "'%s': destroyed", m_name.c_str( ) );
}

void Thread::thread_loop( )
{
   SYS_INF( "'%s': enter", m_name.c_str( ) );
   m_started = true;

   SystemEventConsumer system_event_consumer( *this );

   // Creating components
   for( auto creator : m_component_creators )
      m_components.emplace_back( creator( *this ) );

   while( started( ) )
   {
      base::async::IAsync::tSptr p_event = get_event( );
      SYS_TRC( "'%s': processing event (%s)", m_name.c_str( ), p_event->signature( )->name( ).c_str( ) );
      notify( p_event );
   }

   // Destroying components
   m_components.clear( );

   SYS_INF( "'%s': exit", m_name.c_str( ) );
}

bool Thread::start( )
{
   SYS_INF( "'%s': starting", m_name.c_str( ) );
   bool result = m_thread.run( );
   if( false == result )
   {
      SYS_ERR( "'%s': can't be started", m_name.c_str( ) );
   }

   return result;
}

void Thread::stop( )
{
   SYS_INF( "'%s': stopping", m_name.c_str( ) );
   m_started = false;
}

void Thread::boot( const std::string& message )
{
   SYS_INF( "'%s': booting", m_name.c_str( ) );
   for( auto component : m_components )
      if( component->is_root( ) )
         component->boot( message );
}

void Thread::shutdown( const std::string& message )
{
   SYS_INF( "'%s': shutting down", m_name.c_str( ) );
   stop( );
}

bool Thread::insert_event( const base::async::IAsync::tSptr p_event )
{
   if( false == started( ) )
   {
      SYS_WRN( "'%s': is not started", m_name.c_str( ) );
      return false;
   }

   if( false == is_subscribed( p_event ) )
   {
      SYS_INF( "'%s': there are no consumers for event '%s'", m_name.c_str( ), p_event->signature( )->name( ).c_str( ) )
      return false;
   }

   return m_event_queue.insert( p_event );
}

base::async::IAsync::tSptr Thread::get_event( )
{
   return m_event_queue.get( );
}

void Thread::notify( const base::async::IAsync::tSptr p_event )
{
   // Processing runnable IAsync object
   if( base::async::eAsyncType::RUNNABLE == p_event->signature( )->type( ) )
   {
      process_start( );
      SYS_TRC( "'%s': start processing runnable at %ld (%s)",
            m_name.c_str( ),
            process_started( ),
            p_event->signature( )->name( ).c_str( )
         );
      p_event->process( );
      SYS_TRC( "'%s': finished processing runnable started at %ld (%s)",
            m_name.c_str( ),
            process_started( ),
            p_event->signature( )->name( ).c_str( )
         );
      process_stop( );
      return;
   }

   auto& consumers_set = m_consumers_map.start_process( p_event->signature( ) );
   SYS_TRC( "'%s': %zu consumers will be processed", m_name.c_str( ), consumers_set.size( ) );
   for( base::async::IAsync::IConsumer* p_consumer : consumers_set )
   {
      process_start( );
      SYS_TRC( "'%s': start processing event at %ld (%s)",
            m_name.c_str( ),
            process_started( ),
            p_event->signature( )->name( ).c_str( )
         );
      p_event->process( p_consumer );
      SYS_TRC( "'%s': finished processing event started at %ld (%s)",
            m_name.c_str( ),
            process_started( ),
            p_event->signature( )->name( ).c_str( )
         );
   }
   process_stop( );
   m_consumers_map.finish_process( );
}

void Thread::set_notification( const base::async::IAsync::ISignature::tSptr p_signature, base::async::IAsync::IConsumer* p_consumer )
{
   m_consumers_map.set_notification( p_signature, p_consumer );
}

void Thread::clear_notification( const base::async::IAsync::ISignature::tSptr p_signature, base::async::IAsync::IConsumer* p_consumer )
{
   m_consumers_map.clear_notification( p_signature, p_consumer );
}

void Thread::clear_all_notifications( const base::async::IAsync::ISignature::tSptr p_signature, base::async::IAsync::IConsumer* p_consumer )
{
   m_consumers_map.clear_all_notifications( p_signature, p_consumer );
}

bool Thread::is_subscribed( const base::async::IAsync::tSptr p_event )
{
   if( base::async::eAsyncType::RUNNABLE == p_event->signature( )->type( ) )
      return true;

   return m_consumers_map.is_subscribed( p_event->signature( ) );
}

void Thread::dump( ) const
{
   SYS_DUMP_START( );
   SYS_INF( "%s:", m_name.c_str( ) );
   m_event_queue.dump( );
   m_consumers_map.dump( );
   SYS_DUMP_END( );
}
