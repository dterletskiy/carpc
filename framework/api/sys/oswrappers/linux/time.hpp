#pragma once

#include <cstdint>
#include <ctime>
#include <time.h>
#include <sys/time.h>



namespace base::os::linux {

   void local_time_of_date( tm*&, size_t& );

   uint64_t microseconds( clockid_t clk_id = CLOCK_REALTIME );
   uint64_t nanoseconds( clockid_t clk_id = CLOCK_REALTIME );

} // namespace base::os::linux
