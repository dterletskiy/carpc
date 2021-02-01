#pragma once

#include "api/sys/trace/Types.hpp"



namespace base::trace::linux {

   template< typename... Args >
   void write( const char* _format, Args... _args )
   {
      static thread_local char* p_buffer = (char*)malloc( 1024 );
      auto size = ::sprintf( p_buffer, _format, _args... );
      ::write( STDOUT_FILENO, p_buffer, size );
   }

   extern thread_local tm* time_tm;
   extern thread_local std::size_t milliseconds;

} // namespace base::trace::linux



#define CLASS_ABBR "MAIN"



#define WRITE_CODE( USER_FORMAT, ... ) \
   { \
      base::trace::linux::write( \
           PREFIX_FORMAT_CODE USER_FORMAT NEW_LINE \
         , CLASS_ABBR, __FUNCTION__, __LINE__, ##__VA_ARGS__ \
      ); \
   }

#define WRITE_MICROSECONDS_PID_TID_CODE( USER_FORMAT, ... ) \
   { \
      base::trace::linux::write( \
           PREFIX_FORMAT_MICROSECONDS_PID_TID_CODE USER_FORMAT NEW_LINE \
         , ::base::os::linux::microseconds( ) \
         , getpid( ), pthread_self( ) \
         , CLASS_ABBR, __FUNCTION__, __LINE__, ##__VA_ARGS__ \
      ); \
   }

#define WRITE_DATE_TIME_MILLISECONDS_PID_TID_CODE( USER_FORMAT, ... ) \
   { \
      ::base::os::linux::local_time_of_date( base::trace::linux::time_tm, base::trace::linux::milliseconds ); \
      base::trace::linux::write( \
           PREFIX_FORMAT_DATE_TIME_MILLISECONDS_PID_TID_CODE USER_FORMAT NEW_LINE \
         , base::trace::linux::time_tm->tm_year + 1900, base::trace::linux::time_tm->tm_mon + 1, base::trace::linux::time_tm->tm_mday \
         , base::trace::linux::time_tm->tm_hour, base::trace::linux::time_tm->tm_min, base::trace::linux::time_tm->tm_sec, base::trace::linux::milliseconds \
         , getpid( ), pthread_self( ) \
         , CLASS_ABBR, __FUNCTION__, __LINE__, ##__VA_ARGS__ \
      ); \
   }



#undef CLASS_ABBR
