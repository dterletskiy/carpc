#pragma once

#include <stdio.h>
#include <unistd.h>
#include <iostream>

#if OS_TARGET == OS_ANDROID
   #include <android/log.h>
#endif

#ifdef USE_DLT
   #include <dlt/dlt.h>
#endif

#include "carpc/trace/Base.hpp"



namespace carpc::trace {

   enum class eLogLevel : std::uint8_t { VERBOSE, DEBUG, INFO, WARNING, ERROR, FATAL };
   const char* const to_color( const eLogLevel& );
   #if OS_TARGET == OS_ANDROID
      int to_android( const eLogLevel& );
   #endif
   #ifdef USE_DLT
      DltLogLevelType to_dlt( const eLogLevel& );
   #endif

   enum class eLogStrategy : std::uint8_t { CONSOLE, CONSOLE_EXT, DLT, ANDROID_LOGCAT, UNDEFINED };
   const char* const log_strategy_to_string( const eLogStrategy& );
   const eLogStrategy log_strategy_from_string( const char* const );

}



#include <pthread.h>
#include <sys/time.h>



namespace carpc::trace {

   enum class eGranularity : std::size_t
   {
      seconds = 1,
      miliseconds = 1000,
      microseconds = 1000000,
      nanoseconds = 1000000000
   };

   void local_time_of_date( tm*&, size_t& );
   uint64_t time( const eGranularity gran, clockid_t clk_id = CLOCK_REALTIME );

}
