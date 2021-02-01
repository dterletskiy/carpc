#pragma once

#include <stdio.h>
#include <unistd.h>
#include <iostream>

#if OS == OS_ANDROID
   #include <android/log.h>
#elif OS == OS_LINUX
   #include <dlt/dlt.h>
#endif

#include "api/sys/oswrappers/linux/thread.hpp"
#include "api/sys/oswrappers/linux/time.hpp"
#include "api/sys/trace/Base.hpp"



namespace base::trace {

   enum class eLogLevel : std::uint8_t { VERBOSE, DEBUG, INFO, WARNING, ERROR, FATAL };
   const char* const to_color( const eLogLevel& );
   #if OS == OS_ANDROID
      int to_android( const eLogLevel& );
   #elif OS == OS_LINUX
      DltLogLevelType to_dlt( const eLogLevel& );
   #endif

   enum class eLogStrategy : std::uint8_t { CONSOLE, CONSOLE_EXT, DLT, ANDROID_T, UNDEFINED };

}
