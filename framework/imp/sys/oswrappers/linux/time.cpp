#include "api/sys/oswrappers/linux/time.hpp"


namespace carpc::os::os_linux {



   void local_time_of_date( tm*& _time_tm, std::size_t& _milliseconds )
   {
      timeval tv;
      gettimeofday( &tv, NULL );
      _time_tm = localtime( &tv.tv_sec );
      _milliseconds = tv.tv_usec;
   }

   std::uint64_t time( const eGranularity gran, clockid_t clk_id )
   {
      timespec time_timespec;
      clock_gettime( clk_id, &time_timespec );
      return time_timespec.tv_sec * static_cast< std::size_t >( gran ) +
               time_timespec.tv_nsec / ( 1000000000 / static_cast< std::size_t >( gran ) );
   }

   std::uint64_t seconds( clockid_t clk_id )
   {
      return time( eGranularity::seconds, clk_id );
   }

   std::uint64_t milliseconds( clockid_t clk_id )
   {
      return time( eGranularity::milliseconds, clk_id );
   }

   std::uint64_t microseconds( clockid_t clk_id )
   {
      return time( eGranularity::microseconds, clk_id );
   }

   std::uint64_t nanoseconds( clockid_t clk_id )
   {
      return time( eGranularity::nanoseconds, clk_id );
   }

   timespec timeval_to_timespec( const timeval& tv, timespec* const ts )
   {
      if( nullptr == ts )
         return timespec{ tv.tv_sec, tv.tv_usec * 1000 };

      ts->tv_sec = tv.tv_sec;
      ts->tv_nsec = tv.tv_usec * 1000;
      return *ts;
   }

   timeval timespec_to_timeval( const timespec& ts, timeval* const tv )
   {
      if( nullptr == tv )
         return timeval{ ts.tv_sec, ts.tv_nsec / 1000 };

      tv->tv_sec = ts.tv_sec;
      tv->tv_usec = ts.tv_nsec / 1000;
      return *tv;
   }



} // namespace carpc::os::os_linux
