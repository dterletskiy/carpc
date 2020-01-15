#include "api/sys/oswrappers/linux/timer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Timer"



namespace base::os::linux {



bool create_timer( timer_t& timer_id, int signal )
{
   /* Создаём таймер */
   struct sigevent sev;
   memset( &sev, 0, sizeof(struct sigevent) );
   sev.sigev_notify = SIGEV_SIGNAL;
   sev.sigev_signo = signal;
   sev.sigev_value.sival_ptr = static_cast< void* >( &timer_id );

   int result = timer_create( CLOCK_REALTIME, &sev, &timer_id );
   if( -1 == result )
   {
      int last_errno = errno;
      SYS_ERR( "error: %d", last_errno );
   }

   return 0 == result;
}

bool create_timer( timer_t& timer_id, tEventHandler callback )
{
   /* Создаём таймер */
   struct sigevent sev;
   memset( &sev, 0, sizeof(struct sigevent) );
   sev.sigev_notify = SIGEV_THREAD;
   sev.sigev_notify_function = callback;
   sev.sigev_value.sival_ptr = &timer_id;

   int result = timer_create( CLOCK_REALTIME, &sev, &timer_id );
   if( -1 == result )
   {
      int last_errno = errno;
      SYS_ERR( "error: %d", last_errno );
   }

   return 0 == result;
}

bool delete_timer( const timer_t& timer_id )
{
   int result = timer_delete( timer_id );
   if( -1 == result )
   {
      int last_errno = errno;
      SYS_ERR( "error: %d", last_errno );
   }

   return 0 == result;
}

bool start_timer( const timer_t& timer_id, long int freq_nanosecs, eTimerType type )
{
   /* Запускаем таймер */
   struct itimerspec its;
   memset( &its, 0, sizeof(struct itimerspec) );
   its.it_value.tv_sec = freq_nanosecs / 1000000000;
   its.it_value.tv_nsec = freq_nanosecs % 1000000000;
   if( eTimerType::continious == type )
   {
      its.it_interval.tv_sec = its.it_value.tv_sec;
      its.it_interval.tv_nsec = its.it_value.tv_nsec;
   }

   int result = timer_settime( timer_id, 0, &its, nullptr );
   if( -1 == result )
   {
      int last_errno = errno;
      SYS_ERR( "error: %d", last_errno );
   }

   return 0 == result;
}



} // namespace base::os::linux
