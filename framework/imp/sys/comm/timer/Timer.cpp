#include <map>
#include "api/sys/oswrappers/linux/signals.hpp"
#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/comm/timer/Timer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Timer"



using namespace base::timer;



static base::os::Mutex mutex_consumer_map;
static std::map< base::os::os_linux::timer::tID, Timer* > consumer_map;

namespace {

   void timer_processor( const base::os::os_linux::timer::tID timer_id )
   {
      SYS_VRB( "processing timer: %#lx", (long)timer_id );

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

   void timer_handler( union sigval sv )
   {
      base::os::os_linux::timer::tID* timer_id = static_cast< base::os::os_linux::timer::tID* >( sv.sival_ptr );
      SYS_VRB( "timer id: %#lx", (long)(*timer_id) );
      timer_processor( *timer_id );
   }

   void signal_handler( int signal, siginfo_t* si, void* uc )
   {
      SYS_VRB( "signal: %d / si->si_signo: %d", signal, si->si_signo );
      SYS_VRB( "sival_ptr: %p(%#zx) / sival_int: %d ",
         si->si_value.sival_ptr,
         *static_cast< std::size_t* >( si->si_value.sival_ptr ),
         si->si_value.sival_int
      );

      base::os::os_linux::timer::tID* timer_id = static_cast< base::os::os_linux::timer::tID* >( si->si_value.sival_ptr );
      SYS_VRB( "timer id: %#lx", (long)(*timer_id) );
      timer_processor( *timer_id );
   }

}



Timer::Timer( ITimerConsumer* p_consumer, const std::string& name )
   : m_name( name )
   , mp_consumer( p_consumer )
{
   if( nullptr == mp_consumer )
   {
      SYS_ERR( "Timer consumer is null pointer" );
      return;
   }

   if( false == m_context.is_valid( ) )
   {
      SYS_ERR( "Creating timer not in application thread" );
      return;
   }

   // Variant with "timer_handler" should be used because of in some cases linux can create several timers with the same id
   // in case of using variant with "signal_handler".
   // os::os_linux::signals::set_handler( SIGRTMIN, signal_handler );
   // if( false == os::os_linux::timer::create( m_timer_id, SIGRTMIN ) )
   if( false == os::os_linux::timer::create( m_timer_id, timer_handler ) )
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
      if( false == os::os_linux::timer::remove( m_timer_id ) )
      {
         SYS_ERR( "remove timer error" );
      }
      return;
   }

   SYS_VRB( "created timer: %s(%s -> %#lx)", m_name.c_str( ), m_id.name( ).c_str( ), (long) m_timer_id );
   TimerEvent::Event::set_notification( mp_consumer, { m_id.value( ) } );
}

Timer::~Timer( )
{
   TimerEvent::Event::clear_notification( mp_consumer, { m_id.value( ) } );

   mutex_consumer_map.lock( );
   const std::size_t result = consumer_map.erase( m_timer_id );
   mutex_consumer_map.unlock( );
   if( 0 == result )
   {
      SYS_WRN( "timer has not been found" );
   }
   else if( 1 < result )
   {
      SYS_WRN( "%zu timers have been founded", result );
   }

   if( false == os::os_linux::timer::remove( m_timer_id ) )
   {
      SYS_ERR( "remove timer error" );
   }
   else
   {
      SYS_VRB( "removed timer: %s(%s -> %#lx)", m_name.c_str( ), m_id.name( ).c_str( ), (long) m_timer_id );
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
   os::os_linux::timer::eTimerType type = os::os_linux::timer::eTimerType::single;
   if( CONTINIOUS == m_count ) type = os::os_linux::timer::eTimerType::continious;
   if( false == os::os_linux::timer::start( m_timer_id, (long int)m_nanoseconds, type ) )
   {
      SYS_ERR( "start timer error" );
      return false;
   }

   SYS_VRB( "started timer: %s(%s -> %#lx)", m_name.c_str( ), m_id.name( ).c_str( ), (long) m_timer_id );
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

   if( false == os::os_linux::timer::stop( m_timer_id ) )
   {
      SYS_ERR( "stop timer error" );
      return false;
   }

   SYS_VRB( "stoped timer: %s(%s -> %#lx)", m_name.c_str( ), m_id.name( ).c_str( ), (long) m_timer_id );
   return true;  
}

void Timer::process( const base::os::os_linux::timer::tID id )
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

   TimerEvent::Event::create_send( { m_id.value( ) }, { m_id }, base::priority( ePriority::TIMER ), m_context );
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
#include "api/sys/comm/async/runnable/Runnable.hpp"
#include "api/sys/application/Process.hpp"

namespace base::timer {

   base::comm::timer::ID start( const std::size_t milliseconds, const std::size_t count, tCallback callback, const bool asynchronous )
   {
      application::Context context = application::Context::current( );
      if( application::Context::invalid == context )
      {
         SYS_ERR( "application::Thread has not been found. Creating timer not in application thread" );
         return 0;
      }

      const base::comm::timer::ID id = base::comm::timer::ID::generate( );
      auto on_timer = [=]( ){ callback( id ); };

      if( asynchronous )
      {
         std::thread(
            [=]( )
            {
               for( std::size_t ticks = 0; ticks < count; ++ticks )
               {
                  std::this_thread::sleep_for( std::chrono::milliseconds( milliseconds ) );
                  base::async::Runnable::create_send( on_timer, context );
               }
            }
         ).detach( );
      }
      else
      {
         for( std::size_t ticks = 0; ticks < count; ++ticks )
         {
            std::this_thread::sleep_for( std::chrono::milliseconds( milliseconds ) );
            base::async::Runnable::create_send( on_timer, context );
         }
      }

      return id;
   }

} // namespace base::timer
