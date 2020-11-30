#include <map>
#include "api/sys/oswrappers/linux/signals.hpp"
#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/comm/timer/Timer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Timer"



using namespace base;



static base::os::Mutex mutex_consumer_map;
static std::map< base::os::linux::timer::tID, Timer* > consumer_map;

void timer_processor( const base::os::linux::timer::tID timer_id )
{
   SYS_TRC( "processing timer: %#lx", (long)timer_id );

   mutex_consumer_map.lock( );
   auto iterator = consumer_map.find( timer_id );
   if( consumer_map.end( ) == iterator )
   {
      mutex_consumer_map.unlock( );
      SYS_ERR( "Timer has not been found" );
      return;
   }
   Timer* p_timer = iterator->second;
   mutex_consumer_map.unlock( );

   p_timer->process( timer_id );
}

void signal_handler( int signal, siginfo_t* si, void* uc )
{
   SYS_TRC( "signal: %d / si->si_signo: %d", signal, si->si_signo );
   SYS_TRC( "sival_ptr: %p(%#zx) / sival_int: %d ", si->si_value.sival_ptr, *static_cast< size_t* >( si->si_value.sival_ptr ), si->si_value.sival_int );

   base::os::linux::timer::tID* timer_id = (base::os::linux::timer::tID*)si->si_value.sival_ptr;
   SYS_TRC( "timer id: %#lx", (long)(*timer_id) );
   timer_processor( *timer_id );
}

void event_handler( union sigval sv )
{
   SYS_TRC( "sv.sival_ptr: %p(%#lx) / sv.sival_int: %#x", sv.sival_ptr, *static_cast< size_t* >( sv.sival_ptr ), sv.sival_int );
}



Timer::Timer( ITimerConsumer* p_consumer, const std::string& name )
   : m_name( name )
   , m_id( ID::generate( ) )
   , mp_consumer( p_consumer )
   , m_context( application::Context::eInitType::Auto )
{
   if( nullptr == mp_consumer )
   {
      SYS_ERR( "Timer consumer is null pointer" );
      return;
   }

   if( application::Context::thread::broadcast == m_context.tid( ) )
   {
      SYS_ERR( "application::Thread has not been found. Creating timer not in application thread" );
      return;
   }

   os::linux::signals::set_handler( SIGRTMIN, signal_handler );
   if( false == os::linux::timer::create( m_timer_id, SIGRTMIN ) )
   {
      SYS_ERR( "create timer error" );
      return;
   }

   mutex_consumer_map.lock( );
   const auto [iterator, success] = consumer_map.insert( { m_timer_id, this } );
   mutex_consumer_map.unlock( );
   if( false == success )
   {
      SYS_ERR( "insert error" );
      if( false == os::linux::timer::remove( m_timer_id ) )
      {
         SYS_ERR( "remove timer error" );
      }
      return;
   }

   SYS_TRC( "created timer: %s(%s -> %#lx)", m_name.c_str( ), m_id.name( ).c_str( ), (long) m_timer_id );
   TimerEvent::Event::set_notification( mp_consumer, { m_id.value( ) } );
}

Timer::~Timer( )
{
   TimerEvent::Event::clear_notification( mp_consumer, { m_id.value( ) } );

   mutex_consumer_map.lock( );
   const size_t result = consumer_map.erase( m_timer_id );
   mutex_consumer_map.unlock( );
   if( 0 == result )
   {
      SYS_WRN( "timer has not been found" );
   }
   else if( 1 < result )
   {
      SYS_WRN( "%zu timers have been founded", result );
   }

   if( false == os::linux::timer::remove( m_timer_id ) )
   {
      SYS_ERR( "remove timer error" );
   }
   else
   {
      SYS_TRC( "removed timer: %s(%s -> %#lx)", m_name.c_str( ), m_id.name( ).c_str( ), (long) m_timer_id );
   }
}

const bool Timer::operator<( const Timer& timer ) const
{
   return m_id < timer.m_id;
}

bool Timer::start( const std::size_t nanoseconds, const std::size_t count )
{
   if( true == m_is_running )
   {
      SYS_WRN( "Timer has been started already" );
      return false;
   }

   if( 0 == nanoseconds )
   {
      SYS_ERR( "Zero period value" );
      return false;
   }

   m_nanoseconds = nanoseconds;
   m_count = count;
   m_ticks = 0;
   os::linux::timer::eTimerType type = os::linux::timer::eTimerType::single;
   if( CONTINIOUS == m_count ) type = os::linux::timer::eTimerType::continious;
   if( false == os::linux::timer::start( m_timer_id, (long int)m_nanoseconds, type ) )
   {
      SYS_ERR( "start timer error" );
      return false;
   }

   SYS_TRC( "started timer: %s(%s -> %#lx)", m_name.c_str( ), m_id.name( ).c_str( ), (long) m_timer_id );
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
   m_count = 0;
   m_ticks = 0;

   if( false == os::linux::timer::stop( m_timer_id ) )
   {
      SYS_ERR( "stop timer error" );
      return false;
   }

   SYS_TRC( "stoped timer: %s(%s -> %#lx)", m_name.c_str( ), m_id.name( ).c_str( ), (long) m_timer_id );
   return true;  
}

void Timer::process( const base::os::linux::timer::tID id )
{
   if( id != m_timer_id )
   {
      SYS_ERR( "Timer id mismatch" );
      return;
   }

   ++m_ticks;
   if( m_ticks == m_count )
      stop( );
   else if( m_ticks > m_count )
      return;

   TimerEvent::Event::create_send( { m_id.value( ) }, { m_id }, tPriority( 100 ), m_context );
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







#include <chrono>
#include <functional>
#include <thread>
#include <limits>
#include "api/sys/comm/event/Runnable.hpp"
#include "api/sys/application/Process.hpp"

namespace base::timer {

   base::Timer::ID start( const size_t milliseconds, const size_t count, tCallback callback, const bool asynchronous )
   {
      application::IThread::tSptr p_thread = application::Process::instance( )->current_thread( );
      if( !p_thread )
      {
         SYS_ERR( "application::Thread has not been found. Creating timer not in application thread" );
         return 0;
      }

      const base::Timer::ID id = base::Timer::ID::generate( );
      auto on_timer = [=]( ){ callback( id ); };

      if( asynchronous )
      {
         std::thread(
            [=]( )
            {
               for( size_t ticks = 0; ticks < count; ++ticks )
               {
                  std::this_thread::sleep_for( std::chrono::milliseconds( milliseconds ) );
                  base::async::Runnable::create_send_to_context( on_timer, p_thread );
               }
            }
         ).detach( );
      }
      else
      {
         for( size_t ticks = 0; ticks < count; ++ticks )
         {
            std::this_thread::sleep_for( std::chrono::milliseconds( milliseconds ) );
            base::async::Runnable::create_send_to_context( on_timer, p_thread );
         }
      }

      return id;
   }

} // namespace base::timer
