#pragma once

#include <cstdint>
#include <ctime>
#include <time.h>
#include <sys/time.h>



namespace base::os::os_linux {

   enum class eGranularity : std::size_t
   {
      seconds = 1,
      milliseconds = 1000,
      microseconds = 1000000,
      nanoseconds = 1000000000
   };

   void local_time_of_date( tm*&, std::size_t& );

   std::uint64_t time( const eGranularity gran, clockid_t clk_id = CLOCK_REALTIME );
   std::uint64_t milliseconds( clockid_t clk_id = CLOCK_REALTIME );
   std::uint64_t microseconds( clockid_t clk_id = CLOCK_REALTIME );
   std::uint64_t nanoseconds( clockid_t clk_id = CLOCK_REALTIME );

   timespec timeval_to_timespec( const timeval& tv, timespec* const ts = nullptr );
   timeval timespec_to_timeval( const timespec& ts, timeval* const tv = nullptr );

} // namespace base::os::os_linux
