#include <map>
#include "api/sys/oswrappers/linux/signals.hpp"
#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/service/ServiceThread.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/comm/timer/Timer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Timer"



namespace base {



static base::os::Mutex mutex_consumer_map;
static std::map< TimerID, ServiceThreadPtrW > consumer_map;

// This convert method is used because Event::id could not have time TimerID (aka void*)
const size_t convert( void* id )
{
   return reinterpret_cast< size_t >( id );
}

void timer_processor( const TimerID timer_id )
{
   SYS_TRC( "%#lx: processing timer: %#lx", os::Thread::current_id( ), (long)timer_id );

   mutex_consumer_map.lock( );
   auto iterator = consumer_map.find( timer_id );
   if( consumer_map.end( ) == iterator )
   {
      mutex_consumer_map.unlock( );
      SYS_ERR( "Timer has not been found" );
      return;
   }
   ServiceThreadPtrW pw_service = iterator->second;
   mutex_consumer_map.unlock( );

   TimerEvent::Event::create_send_to_context( convert( timer_id ), { timer_id }, pw_service );
}

void signal_handler( int signal, siginfo_t* si, void* uc )
{
   SYS_TRC( "signal: %d / si->si_signo: %d", signal, si->si_signo );
   SYS_TRC( "sival_ptr: %p(%#zx) / sival_int: %d ", si->si_value.sival_ptr, *static_cast< size_t* >( si->si_value.sival_ptr ), si->si_value.sival_int );

   TimerID* timer_id = (TimerID*)si->si_value.sival_ptr;
   SYS_TRC( "timer id: %#lx", (long)(*timer_id) );
   timer_processor( *timer_id );
}

void event_handler( union sigval sv )
{
   SYS_TRC( "sv.sival_ptr: %p(%#lx) / sv.sival_int: %#x", sv.sival_ptr, *static_cast< size_t* >( sv.sival_ptr ), sv.sival_int );
}



Timer::Timer( ITimerConsumer* p_consumer )
   : mp_consumer( p_consumer )
{
   if( nullptr == mp_consumer )
   {
      SYS_ERR( "Timer consumer is null pointer" );
      return;
   }

   ServiceThreadPtr p_service = ServiceProcess::instance( )->service( os::Thread::current_id( ) );
   if( InvalidServiceThreadPtr == p_service )
   {
      SYS_ERR( "ServiceThread has not been found. Creating timer not in service thread" );
      return;
   }

   os::linux::signals::set_handler( SIGRTMIN, signal_handler );
   if( false == os::linux::timer::create( m_id, SIGRTMIN ) )
   {
      SYS_ERR( "create timer error" );
      return;
   }

   mutex_consumer_map.lock( );
   const auto [iterator, success] = consumer_map.insert( { m_id, p_service } );
   mutex_consumer_map.unlock( );
   if( false == success )
   {
      SYS_ERR( "insert error" );
      if( false == os::linux::timer::remove( m_id ) )
      {
         SYS_ERR( "remove timer error" );
      }
      return;
   }

   SYS_TRC( "created timer: %#lx", (long) m_id );
   TimerEvent::Event::set_notification( mp_consumer, convert( m_id ) );
}
Timer::~Timer( )
{
   TimerEvent::Event::clear_notification( mp_consumer, convert( m_id ) );

   mutex_consumer_map.lock( );
   const size_t result = consumer_map.erase( m_id );
   mutex_consumer_map.unlock( );
   if( 0 == result )
   {
      SYS_WRN( "timer has not been found" );
   }
   else if( 1 < result )
   {
      SYS_WRN( "%zu timers have been founded", result );
   }

   if( false == os::linux::timer::remove( m_id ) )
   {
      SYS_ERR( "remove timer error" );
   }
   else
   {
      SYS_TRC( "removed timer: %#lx", (long) m_id );
   }
}

const bool Timer::operator<( const Timer& timer ) const
{
   return m_id < timer.m_id;
}

bool Timer::start( const long int nanoseconds )
{
   if( true == m_is_running )
   {
      SYS_WRN( "Timer has been started already" );
      return false;
   }

   if( 0 >= nanoseconds )
   {
      SYS_ERR( "Invalid period value: %ld", nanoseconds );
      return false;
   }

   m_nanoseconds = nanoseconds;
   if( false == os::linux::timer::start( m_id, nanoseconds, os::linux::timer::eTimerType::continious ) )
   {
      SYS_ERR( "start timer error" );
      return false;
   }

   SYS_TRC( "started timer: %#lx", (long) m_id );
   m_is_running = true;
   return true;  
}

bool Timer::stop( )
{
   if( false == m_is_running )
   {
      SYS_WRN( "Timer has not been started" );
      return false;
   }

   m_is_running = false;
   m_nanoseconds = 0;

   if( false == os::linux::timer::stop( m_id ) )
   {
      SYS_ERR( "stop timer error" );
      return false;
   }

   SYS_TRC( "stoped timer: %#lx", (long) m_id );
   return true;  
}

bool Timer::is_running( ) const
{
   return m_is_running;
}

const TimerID Timer::id( ) const
{
   return m_id;
}



ITimerConsumer::ITimerConsumer( )
{
}

ITimerConsumer::~ITimerConsumer( )
{
}

void ITimerConsumer::process_event( const TimerEvent::Event& event )
{
   process_timer( event.data( )->id );
}



} // namespace base



INIT_EVENT( base::TimerEvent );
