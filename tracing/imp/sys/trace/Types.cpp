#include "api/sys/trace/Types.hpp"



namespace base::trace {

   const char* const to_color( const eLogLevel& log_level )
   {
      switch( log_level )
      {
         case eLogLevel::VERBOSE:   return FG_WHITE;
         case eLogLevel::DEBUG:     return FG_LIGHT_GREEN;
         case eLogLevel::INFO:      return FG_LIGHT_CYAN;
         case eLogLevel::WARNING:   return FG_LIGHT_BLUE;
         case eLogLevel::ERROR:     return FG_LIGHT_RED;
         case eLogLevel::FATAL:     return FG_RED;
      }
      return FG_WHITE;
   }

   #if OS == OS_ANDROID
   int to_android( const eLogLevel& log_level )
   {
      switch( log_level )
      {
         case eLogLevel::VERBOSE:   return ANDROID_LOG_VERBOSE;
         case eLogLevel::DEBUG:     return ANDROID_LOG_DEBUG;
         case eLogLevel::INFO:      return ANDROID_LOG_INFO;
         case eLogLevel::WARNING:   return ANDROID_LOG_WARN;
         case eLogLevel::ERROR:     return ANDROID_LOG_ERROR;
         case eLogLevel::FATAL:     return ANDROID_LOG_FATAL;
      }
      return ANDROID_LOG_VERBOSE;
   }
   #endif

   #ifdef USE_DLT
   DltLogLevelType to_dlt( const eLogLevel& log_level )
   {
      switch( log_level )
      {
         case eLogLevel::VERBOSE:   return DLT_LOG_INFO; // return DLT_LOG_VERBOSE;
         case eLogLevel::DEBUG:     return DLT_LOG_INFO; // return DLT_LOG_DEBUG;
         case eLogLevel::INFO:      return DLT_LOG_INFO;
         case eLogLevel::WARNING:   return DLT_LOG_WARN;
         case eLogLevel::ERROR:     return DLT_LOG_ERROR;
         case eLogLevel::FATAL:     return DLT_LOG_FATAL;
      }
      return DLT_LOG_VERBOSE;
   }
   #endif

} // namespace base::trace



namespace base::trace {

   void local_time_of_date( tm*& _time_tm, size_t& _milliseconds )
   {
      timeval tv;
      gettimeofday( &tv, NULL );
      _time_tm = localtime( &tv.tv_sec );
      _milliseconds = tv.tv_usec;
   }

   uint64_t time( const eGranularity gran, clockid_t clk_id )
   {
      timespec time_timespec;
      clock_gettime( clk_id, &time_timespec );
      return time_timespec.tv_sec * static_cast< size_t >( gran ) + time_timespec.tv_nsec / ( 1000000000 / static_cast< size_t >( gran ) );
   }

} // namespace base::trace