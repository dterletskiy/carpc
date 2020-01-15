#pragma once

#include <unistd.h>
#include <iostream>

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

   template< typename... Args >
   void message( const char* _format, Args... _args )
   {
      char p_buffer[ 1024 ];
      ::write( STDOUT_FILENO, p_buffer, ::sprintf( p_buffer, _format, _args... ) );
   }

} // namespace base::trace::write



#define CLASS_ABBR "MAIN"



#define DBG_WRITE_CODE_NO_THREAD_SAFE( USER_FORMAT, ... ) \
   base::trace::write::message( \
        PREFIX_FORMAT_CODE USER_FORMAT NEW_LINE \
      , CLASS_ABBR, __FUNCTION__, __LINE__, ##__VA_ARGS__ \
   );
#define DBG_WRITE_PID_TID_CODE_NO_THREAD_SAFE( USER_FORMAT, ... ) \
   base::trace::write::message( \
        PREFIX_FORMAT_MICROSECONDS_PID_TID_CODE USER_FORMAT NEW_LINE \
      , base::os::linux::microseconds( ) \
      , getpid( ), pthread_self( ) \
      , CLASS_ABBR, __FUNCTION__, __LINE__, ##__VA_ARGS__ \
   );



#define DBG_WRITE_CODE( USER_FORMAT, ... ) \
   base::trace::write::simple::dbg_lock( ); \
   ::write( STDOUT_FILENO, base::trace::write::simple::p_buffer, \
      ::sprintf( \
                 base::trace::write::simple::p_buffer \
               , PREFIX_FORMAT_CODE USER_FORMAT NEW_LINE \
               , CLASS_ABBR, __FUNCTION__, __LINE__ \
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
               , base::os::linux::microseconds( ) \
               , getpid( ), pthread_self( ) \
               , CLASS_ABBR, __FUNCTION__, __LINE__ \
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
               , base::trace::write::extended::time_tm->tm_year + 1900, base::trace::write::extended::time_tm->tm_mon + 1, base::trace::write::extended::time_tm->tm_mday \
               , base::trace::write::extended::time_tm->tm_hour, base::trace::write::extended::time_tm->tm_min, base::trace::write::extended::time_tm->tm_sec, base::trace::write::extended::milliseconds \
               , getpid( ), pthread_self( ) \
               , CLASS_ABBR, __FUNCTION__, __LINE__ \
               , ##__VA_ARGS__ \
      ) \
   ); \
   base::trace::write::extended::dbg_unlock( );



#undef CLASS_ABBR
