#pragma once

#include <stdio.h>
#include <iostream>
#include <ctime>
#include <time.h>
#include <sys/time.h>



#define CLR_PREFIX   "\x1B["
#define CLR_SUFFIX   "m"

#define BLACK        "0"
#define RED          "1"
#define GREEN        "2"
#define YELLOW       "3"
#define BLUE         "4"
#define MAGENTA      "5"
#define CYAN         "6"
#define GRAY         "7"

#define FG           "3"
#define FG_LIGHT     "9"
#define BG           "4"
#define BG_LIGHT     "10"




#define RESET              CLR_PREFIX  "0"                  CLR_SUFFIX
#define BOLD               CLR_PREFIX  "1"                  CLR_SUFFIX
#define DIM                CLR_PREFIX  "2"                  CLR_SUFFIX
#define CURSIVE            CLR_PREFIX  "3"                  CLR_SUFFIX
#define UNDERLINE          CLR_PREFIX  "4"                  CLR_SUFFIX
#define BLINK              CLR_PREFIX  "5"                  CLR_SUFFIX
#define REVERSE            CLR_PREFIX  "7"                  CLR_SUFFIX
#define HIDDEN             CLR_PREFIX  "8"                  CLR_SUFFIX

#define FG_BLACK           CLR_PREFIX  FG          BLACK    CLR_SUFFIX
#define FG_RED             CLR_PREFIX  FG          RED      CLR_SUFFIX
#define FG_GREEN           CLR_PREFIX  FG          GREEN    CLR_SUFFIX
#define FG_YELLOW          CLR_PREFIX  FG          YELLOW   CLR_SUFFIX
#define FG_BLUE            CLR_PREFIX  FG          BLUE     CLR_SUFFIX
#define FG_MAGENTA         CLR_PREFIX  FG          MAGENTA  CLR_SUFFIX
#define FG_CYAN            CLR_PREFIX  FG          CYAN     CLR_SUFFIX
#define FG_LIGHT_GRAY      CLR_PREFIX  FG          GRAY     CLR_SUFFIX
#define FG_DARK_GRAY       CLR_PREFIX  FG_LIGHT    BLACK    CLR_SUFFIX
#define FG_LIGHT_RED       CLR_PREFIX  FG_LIGHT    RED      CLR_SUFFIX
#define FG_LIGHT_GREEN     CLR_PREFIX  FG_LIGHT    GREEN    CLR_SUFFIX
#define FG_LIGHT_YELLOW    CLR_PREFIX  FG_LIGHT    YELLOW   CLR_SUFFIX
#define FG_LIGHT_BLUE      CLR_PREFIX  FG_LIGHT    BLUE     CLR_SUFFIX
#define FG_LIGHT_MAGENTA   CLR_PREFIX  FG_LIGHT    MAGENTA  CLR_SUFFIX
#define FG_LIGHT_CYAN      CLR_PREFIX  FG_LIGHT    CYAN     CLR_SUFFIX
#define FG_WHITE           CLR_PREFIX  FG_LIGHT    GRAY     CLR_SUFFIX

#define BG_BLACK           CLR_PREFIX  BG          BLACK    CLR_SUFFIX
#define BG_RED             CLR_PREFIX  BG          RED      CLR_SUFFIX
#define BG_GREEN           CLR_PREFIX  BG          GREEN    CLR_SUFFIX
#define BG_YELLOW          CLR_PREFIX  BG          YELLOW   CLR_SUFFIX
#define BG_BLUE            CLR_PREFIX  BG          BLUE     CLR_SUFFIX
#define BG_MAGENTA         CLR_PREFIX  BG          MAGENTA  CLR_SUFFIX
#define BG_CYAN            CLR_PREFIX  BG          CYAN     CLR_SUFFIX
#define BG_LIGHT_GRAY      CLR_PREFIX  BG          GRAY     CLR_SUFFIX
#define BG_DARK_GRAY       CLR_PREFIX  BG_LIGHT    BLACK    CLR_SUFFIX
#define BG_LIGHT_RED       CLR_PREFIX  BG_LIGHT    RED      CLR_SUFFIX
#define BG_LIGHT_GREEN     CLR_PREFIX  BG_LIGHT    GREEN    CLR_SUFFIX
#define BG_LIGHT_YELLOW    CLR_PREFIX  BG_LIGHT    YELLOW   CLR_SUFFIX
#define BG_LIGHT_BLUE      CLR_PREFIX  BG_LIGHT    BLUE     CLR_SUFFIX
#define BG_LIGHT_MAGENTA   CLR_PREFIX  BG_LIGHT    MAGENTA  CLR_SUFFIX
#define BG_LIGHT_CYAN      CLR_PREFIX  BG_LIGHT    CYAN     CLR_SUFFIX
#define BG_WHITE           CLR_PREFIX  BG_LIGHT    GRAY     CLR_SUFFIX



#define NEW_LINE RESET "\n"
#define CLASS_ABBR "MAIN"

namespace base::trace
{
   void dbg_lock( );
   void dbg_unlock( );

   extern pid_t pid;
   extern pthread_t tid;

   void sys_time( );
   extern tm* time_tm;
   extern size_t milliseconds;
} // namespace base::trace




#define DBG_COMMON_NO( FG_MSG, BG_MSG, USER_FORMAT, ... ) 

#define PREFIX_FORMAT "%s[%s:%s:%d] -> %s%s"
#define DBG_COMMON( FG_MSG, BG_MSG, USER_FORMAT, ... ) \
   base::trace::dbg_lock( ); \
   printf( \
              PREFIX_FORMAT#USER_FORMAT \
            , FG_LIGHT_YELLOW, CLASS_ABBR, __FUNCTION__, __LINE__ \
            , FG_MSG, BG_MSG, ##__VA_ARGS__ ); \
   printf( NEW_LINE ); \
   base::trace::dbg_unlock( );

#define PREFIX_FORMAT_EXT "%s%04d-%02d-%02d %s%02d:%02d:%02d:%06d %s%s %s%#5x %s%#10x %s%s   %s[%s:%s:%d] -> %s%s"
#define DBG_COMMON_EXT( FG_MSG, BG_MSG, USER_FORMAT, ... ) \
   base::trace::dbg_lock( ); \
   base::trace::pid = getpid( ); \
   base::trace::tid = pthread_self( ); \
   base::trace::sys_time( ); \
   printf( \
              PREFIX_FORMAT_EXT#USER_FORMAT \
            , FG_LIGHT_MAGENTA \
               , base::trace::time_tm->tm_year + 1900, base::trace::time_tm->tm_mon + 1, base::trace::time_tm->tm_mday \
            , FG_LIGHT_CYAN \
               , base::trace::time_tm->tm_hour, base::trace::time_tm->tm_min, base::trace::time_tm->tm_sec, base::trace::milliseconds \
            , FG_YELLOW, "|" \
            , FG_RED, getpid( ) \
            , FG_GREEN, pthread_self( ) \
            , FG_YELLOW, "|" \
            , FG_LIGHT_YELLOW, CLASS_ABBR, __FUNCTION__, __LINE__ \
            , FG_MSG, BG_MSG, ##__VA_ARGS__ ); \
   printf( NEW_LINE ); \
   base::trace::dbg_unlock( );


#define SYS_TYPE DBG_COMMON_EXT
#define SYS_BG BG_BLACK
#define SYS_TRC( USER_FORMAT, ... )  SYS_TYPE( FG_WHITE        , SYS_BG, USER_FORMAT, ##__VA_ARGS__ )
#define SYS_INF( USER_FORMAT, ... )  SYS_TYPE( FG_LIGHT_CYAN   , SYS_BG, USER_FORMAT, ##__VA_ARGS__ )
#define SYS_MSG( USER_FORMAT, ... )  SYS_TYPE( FG_LIGHT_GREEN  , SYS_BG, USER_FORMAT, ##__VA_ARGS__ )
#define SYS_WRN( USER_FORMAT, ... )  SYS_TYPE( FG_LIGHT_BLUE   , SYS_BG, USER_FORMAT, ##__VA_ARGS__ )
#define SYS_ERR( USER_FORMAT, ... )  SYS_TYPE( FG_LIGHT_RED    , SYS_BG, USER_FORMAT, ##__VA_ARGS__ )

#define DBG_TYPE DBG_COMMON_EXT
#define DBG_BG BG_BLACK
#define DBG_TRC( USER_FORMAT, ... )  DBG_TYPE( FG_WHITE        , DBG_BG, USER_FORMAT, ##__VA_ARGS__ )
#define DBG_INF( USER_FORMAT, ... )  DBG_TYPE( FG_LIGHT_CYAN   , DBG_BG, USER_FORMAT, ##__VA_ARGS__ )
#define DBG_MSG( USER_FORMAT, ... )  DBG_TYPE( FG_LIGHT_GREEN  , DBG_BG, USER_FORMAT, ##__VA_ARGS__ )
#define DBG_WRN( USER_FORMAT, ... )  DBG_TYPE( FG_LIGHT_BLUE   , DBG_BG, USER_FORMAT, ##__VA_ARGS__ )
#define DBG_ERR( USER_FORMAT, ... )  DBG_TYPE( FG_LIGHT_RED    , DBG_BG, USER_FORMAT, ##__VA_ARGS__ )



#undef CLASS_ABBR
