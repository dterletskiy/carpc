#include "api/sys/comm/async/event/Event.hpp"
#include "api/sys/application/Context.hpp"
#include "api/sys/application/Process.hpp"
#include "api/sys/application/ThreadIPC.hpp"
#include "imp/sys/application/SendReceive.hpp"
#include "imp/sys/application/SystemEventConsumer.hpp"
#include "imp/sys/application/ServiceEventConsumer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "SrvIPC"



using namespace base::application;



ThreadIPC::ThreadIPC( )
   : IThread( "IPC", 10 )
   , m_thread( std::bind( &ThreadIPC::thread_loop, this ) )
   , m_event_queue( Process::instance( )->configuration( ).max_priority, "IPC" )
   , m_consumers_map( "IPC" )
{
   SYS_VRB( "'%s': created", m_name.c_str( ) );
   mp_send_receive = new SendReceive;
}

ThreadIPC::~ThreadIPC( )
{
   SYS_VRB( "'%s': destroyed", m_name.c_str( ) );
   delete mp_send_receive;
}

void ThreadIPC::thread_loop( )
{
   SYS_INF( "'%s': enter", m_name.c_str( ) );
   m_started.store( true );

   SystemEventConsumer system_event_consumer( *this );
   ServiceEventConsumer service_event_consumer( mp_send_receive );

   while( started( ) )
   {
      base::async::IAsync::tSptr p_event = get_event( );
      SYS_VRB( "'%s': processing event (%s)", m_name.c_str( ), p_event->signature( )->name( ).c_str( ) );
      notify( p_event );
   }

   SYS_INF( "'%s': exit", m_name.c_str( ) );
}

bool ThreadIPC::start( )
{
   SYS_INF( "'%s': starting", m_name.c_str( ) );
   if( false == m_thread.run( m_name ) )
   {
      SYS_ERR( "'%s': can't be started", m_name.c_str( ) );
      return false;
   }

   if( false == mp_send_receive->start( ) )
   {
      SYS_ERR( "'%s': can't be started", m_name.c_str( ) );
      return false;
   }

   return true;
}

void ThreadIPC::stop( )
{
   SYS_INF( "'%s': stopping", m_name.c_str( ) );
   m_started.store( false );
   mp_send_receive->stop( );
}

bool ThreadIPC::wait( )
{
   const bool started = m_thread.join( );
   m_started.store( started );
   const bool stopped = mp_send_receive->wait( );
   return !m_started.load( ) && !stopped;
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
      SYS_INF( "'%s': there are no consumers for event '%s'", m_name.c_str( ), p_event->signature( )->name( ).c_str( ) );
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
   switch( p_event->type( ) )
   {
      case async::eAsyncType::CALLABLE:
      case async::eAsyncType::RUNNABLE:
      {
         process_start( );
         SYS_VRB( "'%s': start processing runnable at %ld (%s)",
               m_name.c_str( ),
               process_started( ),
               p_event->signature( )->name( ).c_str( )
            );
         p_event->process( );
         SYS_VRB( "'%s': finished processing runnable started at %ld (%s)",
               m_name.c_str( ),
               process_started( ),
               p_event->signature( )->name( ).c_str( )
            );
         process_stop( );

         break;
      }
      case async::eAsyncType::EVENT:
      {
         auto& consumers_set = m_consumers_map.start_process( p_event->signature( ) );
         SYS_VRB( "'%s': %zu consumers will be processed", m_name.c_str( ), consumers_set.size( ) );
         for( base::async::IAsync::IConsumer* p_consumer : consumers_set )
         {
            process_start( );
            SYS_VRB( "'%s': start processing event at %ld (%s)",
                  m_name.c_str( ),
                  process_started( ),
                  p_event->signature( )->name( ).c_str( )
               );
            p_event->process( p_consumer );
            SYS_VRB( "'%s': finished processing event started at %ld (%s)",
                  m_name.c_str( ),
                  process_started( ),
                  p_event->signature( )->name( ).c_str( )
               );
         }
         process_stop( );
         m_consumers_map.finish_process( );

         break;
      }
      default: break;
   }
}

void ThreadIPC::set_notification(
            const base::async::IAsync::ISignature::tSptr p_signature, base::async::IAsync::IConsumer* p_consumer
         )
{
   m_consumers_map.set_notification( p_signature, p_consumer );
}

void ThreadIPC::clear_notification(
            const base::async::IAsync::ISignature::tSptr p_signature, base::async::IAsync::IConsumer* p_consumer
         )
{
   m_consumers_map.clear_notification( p_signature, p_consumer );
}

void ThreadIPC::clear_all_notifications(
            const base::async::IAsync::ISignature::tSptr p_signature, base::async::IAsync::IConsumer* p_consumer
         )
{
   m_consumers_map.clear_all_notifications( p_signature, p_consumer );
}

bool ThreadIPC::is_subscribed( const base::async::IAsync::tSptr p_event )
{
   switch( p_event->type( ) )
   {
      case async::eAsyncType::CALLABLE:
      case async::eAsyncType::RUNNABLE:   return true;
      case async::eAsyncType::EVENT:      return m_consumers_map.is_subscribed( p_event->signature( ) );
   }
   return false;
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
   return mp_send_receive->send( std::static_pointer_cast< async::IEvent >( p_event ), to_context );
}
