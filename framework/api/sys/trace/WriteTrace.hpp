#pragma once

#include <stdio.h>
#include <unistd.h>

#include "api/sys/oswrappers/linux/time.hpp"
#include "api/sys/trace/Base.hpp"



namespace base::trace::write {

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

} // namespace base::trace::write



#define CLASS_ABBR "MAIN"



#ifndef NO_TRACE

#define DBG_WRITE_CODE( USER_FORMAT, ... ) \
   base::trace::write::simple::dbg_lock( ); \
   ::write( STDOUT_FILENO, base::trace::write::simple::p_buffer, \
      ::sprintf( \
                 base::trace::write::simple::p_buffer \
               , PREFIX_FORMAT_CODE USER_FORMAT NEW_LINE \
               , FG_LIGHT_YELLOW, CLASS_ABBR, __FUNCTION__, __LINE__ \
               , ##__VA_ARGS__ \
      ) \
   ); \
   base::trace::write::simple::dbg_unlock( );

#define DBG_WRITE_MICROSECONDS_PID_TID_CODE( USER_FORMAT, ... ) \
   base::trace::write::extended::dbg_lock( ); \
   ::write( STDOUT_FILENO, base::trace::write::extended::p_buffer, \
      ::sprintf( \
                 base::trace::write::extended::p_buffer \
               , PREFIX_FORMAT_MICROSECONDS_PID_TID_CODE USER_FORMAT NEW_LINE \
               , FG_LIGHT_MAGENTA, base::os::linux::microseconds( ) \
               , FG_YELLOW, "|" \
               , FG_RED, getpid( ) \
               , FG_GREEN, pthread_self( ) \
               , FG_YELLOW, "|" \
               , FG_LIGHT_YELLOW, CLASS_ABBR, __FUNCTION__, __LINE__ \
               , ##__VA_ARGS__ \
      ) \
   ); \
   base::trace::write::extended::dbg_unlock( );

#define DBG_WRITE_DATE_TIME_MILLISECONDS_PID_TID_CODE( USER_FORMAT, ... ) \
   base::trace::write::extended::dbg_lock( ); \
   base::os::linux::local_time_of_date( base::trace::write::extended::time_tm, base::trace::write::extended::milliseconds ); \
   ::write( STDOUT_FILENO, base::trace::write::extended::p_buffer, \
      ::sprintf( \
                 base::trace::write::extended::p_buffer \
               , PREFIX_FORMAT_DATE_TIME_MILLISECONDS_PID_TID_CODE USER_FORMAT NEW_LINE \
               , FG_LIGHT_MAGENTA \
                  , base::trace::write::extended::time_tm->tm_year + 1900, base::trace::write::extended::time_tm->tm_mon + 1, base::trace::write::extended::time_tm->tm_mday \
               , FG_LIGHT_CYAN \
                  , base::trace::write::extended::time_tm->tm_hour, base::trace::write::extended::time_tm->tm_min, base::trace::write::extended::time_tm->tm_sec, base::trace::write::extended::milliseconds \
               , FG_YELLOW, "|" \
               , FG_RED, getpid( ) \
               , FG_GREEN, pthread_self( ) \
               , FG_YELLOW, "|" \
               , FG_LIGHT_YELLOW, CLASS_ABBR, __FUNCTION__, __LINE__ \
               , ##__VA_ARGS__ \
      ) \
   ); \
   base::trace::write::extended::dbg_unlock( );



#else // #ifndef NO_TRACE



#define DBG_WRITE_CODE( USER_FORMAT, ... )
#define DBG_WRITE_MICROSECONDS_PID_TID_CODE( USER_FORMAT, ... )
#define DBG_WRITE_DATE_TIME_MILLISECONDS_PID_TID_CODE( USER_FORMAT, ... )

#endif // #ifndef NO_TRACE



#undef CLASS_ABBR
