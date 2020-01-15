#pragma once

#include <stdio.h>

#include "api/sys/oswrappers/linux/time.hpp"
#include "api/sys/trace/Base.hpp"



namespace base::trace::printf
{
   namespace simple {

      void dbg_lock( );
      void dbg_unlock( );

      extern char p_buffer[ 1024 ];

   }

   namespace extended {

      void dbg_lock( );
      void dbg_unlock( );

      extern char p_buffer[ 1024 ];

      extern tm* time_tm;
      extern size_t milliseconds;

   }

} // namespace base::trace::printf



#define CLASS_ABBR "MAIN"



#ifndef NO_TRACE

#define DBG_PRINTF_CODE( USER_FORMAT, ... ) \
   base::trace::printf::simple::dbg_lock( ); \
   ::printf( \
              PREFIX_FORMAT_CODE#USER_FORMAT \
            , FG_LIGHT_YELLOW, CLASS_ABBR, __FUNCTION__, __LINE__ \
            , ##__VA_ARGS__ \
   ); \
   ::printf( NEW_LINE ); \
   base::trace::printf::simple::dbg_unlock( );

#define DBG_PRINTF_MICROSECONDS_PID_TID_CODE( USER_FORMAT, ... ) \
   base::trace::printf::extended::dbg_lock( ); \
   ::printf( \
              PREFIX_FORMAT_MICROSECONDS_PID_TID_CODE#USER_FORMAT \
            , FG_LIGHT_MAGENTA, base::os::linux::microseconds( ) \
            , FG_YELLOW, "|" \
            , FG_RED, getpid( ) \
            , FG_GREEN, pthread_self( ) \
            , FG_YELLOW, "|" \
            , FG_LIGHT_YELLOW, CLASS_ABBR, __FUNCTION__, __LINE__ \
            , ##__VA_ARGS__ \
   ); \
   ::printf( NEW_LINE ); \
   base::trace::printf::extended::dbg_unlock( );

#define DBG_PRINTF_DATE_TIME_MILLISECONDS_PID_TID_CODE( USER_FORMAT, ... ) \
   base::trace::printf::extended::dbg_lock( ); \
   base::os::linux::local_time_of_date( base::trace::printf::extended::time_tm, base::trace::printf::extended::milliseconds ); \
   ::printf( \
              PREFIX_FORMAT_DATE_TIME_MILLISECONDS_PID_TID_CODE#USER_FORMAT \
            , FG_LIGHT_MAGENTA \
               , base::trace::printf::extended::time_tm->tm_year + 1900, base::trace::printf::extended::time_tm->tm_mon + 1, base::trace::printf::extended::time_tm->tm_mday \
            , FG_LIGHT_CYAN \
               , base::trace::printf::extended::time_tm->tm_hour, base::trace::printf::extended::time_tm->tm_min, base::trace::printf::extended::time_tm->tm_sec, base::trace::printf::extended::milliseconds \
            , FG_YELLOW, "|" \
            , FG_RED, getpid( ) \
            , FG_GREEN, pthread_self( ) \
            , FG_YELLOW, "|" \
            , FG_LIGHT_YELLOW, CLASS_ABBR, __FUNCTION__, __LINE__ \
            , ##__VA_ARGS__ \
   ); \
   ::printf( NEW_LINE ); \
   base::trace::printf::extended::dbg_unlock( );



#else // #ifndef NO_TRACE

#define DBG_PRINTF_CODE( USER_FORMAT, ... )
#define DBG_PRINTF_MICROSECONDS_PID_TID_CODE( USER_FORMAT, ... )
#define DBG_PRINTF_DATE_TIME_MILLISECONDS_PID_TID_CODE( USER_FORMAT, ... )

#endif // #ifndef NO_TRACE



#undef CLASS_ABBR
