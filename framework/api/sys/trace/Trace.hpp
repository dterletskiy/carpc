#pragma once

#include "api/sys/trace/Logger.hpp"



#define USE_NEW_TRACING

#ifdef USE_NEW_TRACING

   #ifdef SYS_TRACE
      #define SYS_TYPE TRACE_LOG
   #else
      #define SYS_TYPE( ... )
   #endif

   #ifdef MSG_TRACE
      #define MSG_TYPE TRACE_LOG
   #else
      #define MSG_TYPE( ... )
   #endif

   #define SYS_VRB( USER_FORMAT, ... )          SYS_TYPE( base::trace::eLogLevel::VERBOSE,   USER_FORMAT, ##__VA_ARGS__ )
   #define SYS_DBG( USER_FORMAT, ... )          SYS_TYPE( base::trace::eLogLevel::DEBUG,     USER_FORMAT, ##__VA_ARGS__ )
   #define SYS_INF( USER_FORMAT, ... )          SYS_TYPE( base::trace::eLogLevel::INFO,      USER_FORMAT, ##__VA_ARGS__ )
   #define SYS_WRN( USER_FORMAT, ... )          SYS_TYPE( base::trace::eLogLevel::WARNING,   USER_FORMAT, ##__VA_ARGS__ )
   #define SYS_ERR( USER_FORMAT, ... )          SYS_TYPE( base::trace::eLogLevel::ERROR,     USER_FORMAT, ##__VA_ARGS__ )
   #define SYS_FTL( USER_FORMAT, ... )          SYS_TYPE( base::trace::eLogLevel::FATAL,     USER_FORMAT, ##__VA_ARGS__ )

   #define MSG_VRB( USER_FORMAT, ... )          MSG_TYPE( base::trace::eLogLevel::VERBOSE,   USER_FORMAT, ##__VA_ARGS__ )
   #define MSG_DBG( USER_FORMAT, ... )          MSG_TYPE( base::trace::eLogLevel::DEBUG,     USER_FORMAT, ##__VA_ARGS__ )
   #define MSG_INF( USER_FORMAT, ... )          MSG_TYPE( base::trace::eLogLevel::INFO,      USER_FORMAT, ##__VA_ARGS__ )
   #define MSG_WRN( USER_FORMAT, ... )          MSG_TYPE( base::trace::eLogLevel::WARNING,   USER_FORMAT, ##__VA_ARGS__ )
   #define MSG_ERR( USER_FORMAT, ... )          MSG_TYPE( base::trace::eLogLevel::ERROR,     USER_FORMAT, ##__VA_ARGS__ )
   #define MSG_FTL( USER_FORMAT, ... )          MSG_TYPE( base::trace::eLogLevel::FATAL,     USER_FORMAT, ##__VA_ARGS__ )

#else

   #if OS == OS_ANDROID

      #include "api/sys/trace/TraceAndroid.hpp"



      #define SYS_VRB( USER_FORMAT, ... )          TRACE_ANDROID( ANDROID_LOG_VERBOSE,   USER_FORMAT, ##__VA_ARGS__ )
      #define SYS_INF( USER_FORMAT, ... )          TRACE_ANDROID( ANDROID_LOG_INFO,      USER_FORMAT, ##__VA_ARGS__ )
      #define SYS_DBG( USER_FORMAT, ... )          TRACE_ANDROID( ANDROID_LOG_DEBUG,     USER_FORMAT, ##__VA_ARGS__ )
      #define SYS_WRN( USER_FORMAT, ... )          TRACE_ANDROID( ANDROID_LOG_WARN,      USER_FORMAT, ##__VA_ARGS__ )
      #define SYS_ERR( USER_FORMAT, ... )          TRACE_ANDROID( ANDROID_LOG_ERROR,     USER_FORMAT, ##__VA_ARGS__ )

      #define MSG_VRB( USER_FORMAT, ... )          TRACE_ANDROID( ANDROID_LOG_VERBOSE,   USER_FORMAT, ##__VA_ARGS__ )
      #define MSG_INF( USER_FORMAT, ... )          TRACE_ANDROID( ANDROID_LOG_INFO,      USER_FORMAT, ##__VA_ARGS__ )
      #define MSG_DBG( USER_FORMAT, ... )          TRACE_ANDROID( ANDROID_LOG_DEBUG,     USER_FORMAT, ##__VA_ARGS__ )
      #define MSG_WRN( USER_FORMAT, ... )          TRACE_ANDROID( ANDROID_LOG_WARN,      USER_FORMAT, ##__VA_ARGS__ )
      #define MSG_ERR( USER_FORMAT, ... )          TRACE_ANDROID( ANDROID_LOG_ERROR,     USER_FORMAT, ##__VA_ARGS__ )

   #elif OS == OS_LINUX

      #include "api/sys/trace/TraceLinux.hpp"



      #ifdef SYS_TRACE
         #define SYS_TYPE WRITE_MICROSECONDS_PID_TID_CODE
      #else
         #define SYS_TYPE( ... )
      #endif

      #ifdef MSG_TRACE
         #define MSG_TYPE WRITE_MICROSECONDS_PID_TID_CODE
      #else
         #define MSG_TYPE( ... )
      #endif

      #define SYS_BG BG_BLACK CURSIVE
      #define SYS_VRB( USER_FORMAT, ... )          SYS_TYPE( FG_WHITE         SYS_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define SYS_INF( USER_FORMAT, ... )          SYS_TYPE( FG_LIGHT_CYAN    SYS_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define SYS_DBG( USER_FORMAT, ... )          SYS_TYPE( FG_LIGHT_GREEN   SYS_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define SYS_WRN( USER_FORMAT, ... )          SYS_TYPE( FG_LIGHT_BLUE    SYS_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define SYS_ERR( USER_FORMAT, ... )          SYS_TYPE( FG_LIGHT_RED     SYS_BG     USER_FORMAT, ##__VA_ARGS__ )

      #define MSG_BG BG_BLACK
      #define MSG_VRB( USER_FORMAT, ... )          MSG_TYPE( FG_WHITE         MSG_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define MSG_INF( USER_FORMAT, ... )          MSG_TYPE( FG_LIGHT_CYAN    MSG_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define MSG_DBG( USER_FORMAT, ... )          MSG_TYPE( FG_LIGHT_GREEN   MSG_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define MSG_WRN( USER_FORMAT, ... )          MSG_TYPE( FG_LIGHT_BLUE    MSG_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define MSG_ERR( USER_FORMAT, ... )          MSG_TYPE( FG_LIGHT_RED     MSG_BG     USER_FORMAT, ##__VA_ARGS__ )



      #ifdef SYS_TRACE
         #define SYS_SIMPLE_TYPE WRITE_CODE
      #else
         #define SYS_SIMPLE_TYPE( ... )
      #endif

      #ifdef MSG_TRACE
         #define MSG_SIMPLE_TYPE WRITE_CODE
      #else
         #define MSG_SIMPLE_TYPE( ... )
      #endif

      #define SYS_SIMPLE_BG BG_BLACK
      #define SYS_VRB_SIMPLE( USER_FORMAT, ... )   SYS_SIMPLE_TYPE( FG_WHITE         SYS_SIMPLE_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define SYS_INF_SIMPLE( USER_FORMAT, ... )   SYS_SIMPLE_TYPE( FG_LIGHT_CYAN    SYS_SIMPLE_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define SYS_DBG_SIMPLE( USER_FORMAT, ... )   SYS_SIMPLE_TYPE( FG_LIGHT_GREEN   SYS_SIMPLE_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define SYS_WRN_SIMPLE( USER_FORMAT, ... )   SYS_SIMPLE_TYPE( FG_LIGHT_BLUE    SYS_SIMPLE_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define SYS_ERR_SIMPLE( USER_FORMAT, ... )   SYS_SIMPLE_TYPE( FG_LIGHT_RED     SYS_SIMPLE_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define SYS_FTL_SIMPLE( USER_FORMAT, ... )   SYS_SIMPLE_TYPE( FG_RED           SYS_SIMPLE_BG     USER_FORMAT, ##__VA_ARGS__ )

      #define MSG_SIMPLE_BG BG_BLACK
      #define MSG_VRB_SIMPLE( USER_FORMAT, ... )   MSG_SIMPLE_TYPE( FG_WHITE         MSG_SIMPLE_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define MSG_INF_SIMPLE( USER_FORMAT, ... )   MSG_SIMPLE_TYPE( FG_LIGHT_CYAN    MSG_SIMPLE_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define MSG_DBG_SIMPLE( USER_FORMAT, ... )   MSG_SIMPLE_TYPE( FG_LIGHT_GREEN   MSG_SIMPLE_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define MSG_WRN_SIMPLE( USER_FORMAT, ... )   MSG_SIMPLE_TYPE( FG_LIGHT_BLUE    MSG_SIMPLE_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define MSG_ERR_SIMPLE( USER_FORMAT, ... )   MSG_SIMPLE_TYPE( FG_LIGHT_RED     MSG_SIMPLE_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define MSG_FTL_SIMPLE( USER_FORMAT, ... )   MSG_SIMPLE_TYPE( FG_RED           MSG_SIMPLE_BG     USER_FORMAT, ##__VA_ARGS__ )



      #ifdef SYS_TRACE
         #define SYS_EXT_TYPE WRITE_DATE_TIME_MILLISECONDS_PID_TID_CODE
      #else
         #define SYS_EXT_TYPE( ... )
      #endif

      #ifdef MSG_TRACE
         #define MSG_EXT_TYPE WRITE_DATE_TIME_MILLISECONDS_PID_TID_CODE
      #else
         #define MSG_EXT_TYPE( ... )
      #endif

      #define SYS_EXT_BG BG_BLACK
      #define SYS_VRB_EXT( USER_FORMAT, ... )      SYS_EXT_TYPE( FG_WHITE         SYS_EXT_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define SYS_INF_EXT( USER_FORMAT, ... )      SYS_EXT_TYPE( FG_LIGHT_CYAN    SYS_EXT_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define SYS_DBG_EXT( USER_FORMAT, ... )      SYS_EXT_TYPE( FG_LIGHT_GREEN   SYS_EXT_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define SYS_WRN_EXT( USER_FORMAT, ... )      SYS_EXT_TYPE( FG_LIGHT_BLUE    SYS_EXT_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define SYS_ERR_EXT( USER_FORMAT, ... )      SYS_EXT_TYPE( FG_LIGHT_RED     SYS_EXT_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define SYS_FTL_EXT( USER_FORMAT, ... )      SYS_EXT_TYPE( FG_RED           SYS_EXT_BG     USER_FORMAT, ##__VA_ARGS__ )

      #define MSG_EXT_BG BG_BLACK
      #define MSG_VRB_EXT( USER_FORMAT, ... )      MSG_EXT_TYPE( FG_WHITE         MSG_EXT_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define MSG_INF_EXT( USER_FORMAT, ... )      MSG_EXT_TYPE( FG_LIGHT_CYAN    MSG_EXT_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define MSG_DBG_EXT( USER_FORMAT, ... )      MSG_EXT_TYPE( FG_LIGHT_GREEN   MSG_EXT_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define MSG_WRN_EXT( USER_FORMAT, ... )      MSG_EXT_TYPE( FG_LIGHT_BLUE    MSG_EXT_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define MSG_ERR_EXT( USER_FORMAT, ... )      MSG_EXT_TYPE( FG_LIGHT_RED     MSG_EXT_BG     USER_FORMAT, ##__VA_ARGS__ )
      #define MSG_FTL_EXT( USER_FORMAT, ... )      MSG_EXT_TYPE( FG_RED           MSG_EXT_BG     USER_FORMAT, ##__VA_ARGS__ )

   #endif

#endif





#define TRACE_MARKER_LINE        "----------------------------------------------"

#define SYS_MARKER( VALUE )      SYS_ERR( TRACE_MARKER_LINE " MARKER: %s " TRACE_MARKER_LINE, #VALUE )
#define MSG_MARKER( VALUE )      MSG_ERR( TRACE_MARKER_LINE " MARKER: %s " TRACE_MARKER_LINE, #VALUE )

#define SYS_DUMP_START( VALUE )  SYS_WRN( TRACE_MARKER_LINE " START DUMP: %s " TRACE_MARKER_LINE, #VALUE )
#define SYS_DUMP_END( VALUE )    SYS_WRN( TRACE_MARKER_LINE "  END DUMP:  %s " TRACE_MARKER_LINE, #VALUE )
#define MSG_DUMP_START( VALUE )  MSG_WRN( TRACE_MARKER_LINE " START DUMP: %s " TRACE_MARKER_LINE, #VALUE )
#define MSG_DUMP_END( VALUE )    MSG_WRN( TRACE_MARKER_LINE "  END DUMP:  %s " TRACE_MARKER_LINE, #VALUE )

#define SYS_SIZEOF( TYPE )       SYS_DBG( "sizeof( %s ) = %zu", #TYPE, sizeof( TYPE ) );
#define MSG_SIZEOF( TYPE )       MSG_DBG( "sizeof( %s ) = %zu", #TYPE, sizeof( TYPE ) );

#define SYS_TRACE_INFORMATION \
   SYS_VRB( "system verbose level message" ); \
   SYS_INF( "system information level message" ); \
   SYS_DBG( "system debug level message" ); \
   SYS_WRN( "system warning level message" ); \
   SYS_ERR( "system error level message" ); \
   SYS_FTL( "system fatal level message" );

#define MSG_TRACE_INFORMATION \
   MSG_VRB( "application verbose level message" ); \
   MSG_INF( "application information level message" ); \
   MSG_DBG( "application debug level message" ); \
   MSG_WRN( "application warning level message" ); \
   MSG_ERR( "application error level message" ); \
   MSG_FTL( "application fatal level message" );

