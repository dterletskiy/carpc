#include "api/sys/comm/event/Event.hpp"
#include "api/sys/application/Context.hpp"
#include "api/sys/application/Process.hpp"
#include "api/sys/application/ThreadIPC.hpp"
#include "imp/sys/application/SystemEventConsumer.hpp"
#include "imp/sys/application/ServiceEventConsumer.hpp"
#include "api/sys/dsi/Types.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "SrvIPC"



using namespace base::application;



ThreadIPC::ThreadIPC( )
   : IThread( "IPC", 10 )
   , m_thread( std::bind( &ThreadIPC::thread_loop, this ) )
   , m_event_queue( Process::instance( )->configuration( ).max_priority, "IPC" )
   , m_consumers_map( "IPC" )
{
   SYS_TRC( "'%s': created", m_name.c_str( ) );
}

ThreadIPC::~ThreadIPC( )
{
   SYS_TRC( "'%s': destroyed", m_name.c_str( ) );
}

void ThreadIPC::thread_loop( )
{
   SYS_INF( "'%s': enter", m_name.c_str( ) );
   m_started = true;

   SystemEventConsumer system_event_consumer( *this );
   ServiceEventConsumer service_event_consumer( *this );

   while( started( ) )
   {
      base::async::IAsync::tSptr p_event = get_event( );
      SYS_TRC( "'%s': processing event (%s)", m_name.c_str( ), p_event->signature( )->name( ).c_str( ) );
      notify( p_event );
   }

   SYS_INF( "'%s': exit", m_name.c_str( ) );
}

bool ThreadIPC::start( )
{
   SYS_INF( "'%s': starting", m_name.c_str( ) );
   if( false == m_thread.run( ) )
   {
      SYS_ERR( "'%s': can't be started", m_name.c_str( ) );
      return false;
   }

   if( false == m_send_receive.start( ) )
   {
      SYS_ERR( "'%s': can't be started", m_name.c_str( ) );
      return false;
   }

   return true;
}

void ThreadIPC::stop( )
{
   SYS_INF( "'%s': stopping", m_name.c_str( ) );
   m_started = false;
   m_send_receive.stop( );
}

void ThreadIPC::boot( const std::string& message )
{
   SYS_INF( "'%s': booting", m_name.c_str( ) );
}

void ThreadIPC::shutdown( const std::string& message )
{
   SYS_INF( "'%s': shutting down", m_name.c_str( ) );
   stop( );
}

bool ThreadIPC::insert_event( const base::async::IAsync::tSptr p_event )
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

base::async::IAsync::tSptr ThreadIPC::get_event( )
{
   return m_event_queue.get( );
}

void ThreadIPC::notify( const base::async::IAsync::tSptr p_event )
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

void ThreadIPC::set_notification( const base::async::IAsync::ISignature::tSptr p_signature, base::async::IAsync::IConsumer* p_consumer )
{
   m_consumers_map.set_notification( p_signature, p_consumer );
}

void ThreadIPC::clear_notification( const base::async::IAsync::ISignature::tSptr p_signature, base::async::IAsync::IConsumer* p_consumer )
{
   m_consumers_map.clear_notification( p_signature, p_consumer );
}

void ThreadIPC::clear_all_notifications( const base::async::IAsync::ISignature::tSptr p_signature, base::async::IAsync::IConsumer* p_consumer )
{
   m_consumers_map.clear_all_notifications( p_signature, p_consumer );
}

bool ThreadIPC::is_subscribed( const base::async::IAsync::tSptr p_event )
{
   if( base::async::eAsyncType::RUNNABLE == p_event->signature( )->type( ) )
      return true;

   return m_consumers_map.is_subscribed( p_event->signature( ) );
}

void ThreadIPC::dump( ) const
{
   SYS_DUMP_START( );
   SYS_INF( "%s:", m_name.c_str( ) );
   m_event_queue.dump( );
   m_consumers_map.dump( );
   SYS_DUMP_END( );
}

bool ThreadIPC::send( const base::async::IAsync::tSptr p_event, const application::Context& to_context )
{
   return m_send_receive.send( std::static_pointer_cast< async::IEvent >( p_event ), to_context );
}

bool ThreadIPC::send( const dsi::Packet& packet, const application::Context& to_context )
{
   return m_send_receive.send( packet, to_context );
}
