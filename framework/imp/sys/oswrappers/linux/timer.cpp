#include "api/sys/oswrappers/linux/timer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "timer"



namespace carpc::os::os_linux::timer {



   bool create( tID& timer_id, int signal )
   {
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

      SYS_VRB( "%#lx", (long)timer_id );
      SYS_VRB( "%p -> %#lx", sev.sigev_value.sival_ptr, *static_cast< long* >( sev.sigev_value.sival_ptr ) );
      return 0 == result;
   }

   bool create( tID& timer_id, tEventHandler callback, void* p_attributes )
   {
      struct sigevent sev;
      memset( &sev, 0, sizeof(struct sigevent) );
      sev.sigev_notify = SIGEV_THREAD;
      sev.sigev_notify_function = callback;
      sev.sigev_value.sival_ptr = static_cast< void* >( &timer_id );
      sev.sigev_notify_attributes = static_cast< pthread_attr_t* >( p_attributes );

      int result = timer_create( CLOCK_REALTIME, &sev, &timer_id );
      if( -1 == result )
      {
         int last_errno = errno;
         SYS_ERR( "error: %d", last_errno );
      }

      SYS_VRB( "%#lx", (long)timer_id );
      SYS_VRB( "%p -> %#lx", sev.sigev_value.sival_ptr, *static_cast< long* >( sev.sigev_value.sival_ptr ) );
      return 0 == result;
   }

   bool remove( const tID& timer_id )
   {
      int result = timer_delete( timer_id );
      if( -1 == result )
      {
         int last_errno = errno;
         SYS_ERR( "error: %d", last_errno );
      }

      SYS_VRB( "%#lx", (long)timer_id );
      return 0 == result;
   }

   bool start( const tID& timer_id, long int freq_nanosecs, eTimerType type )
   {
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

      SYS_VRB( "%#lx", (long)timer_id );
      return 0 == result;
   }

   bool stop( const tID& timer_id )
   {
      struct itimerspec its;
      memset( &its, 0, sizeof(struct itimerspec) );

      int result = timer_settime( timer_id, 0, &its, nullptr );
      if( -1 == result )
      {
         int last_errno = errno;
         SYS_ERR( "error: %d", last_errno );
      }

      SYS_VRB( "%#lx", (long)timer_id );
      return 0 == result;
   }



} // namespace carpc::os::os_linux::timer
