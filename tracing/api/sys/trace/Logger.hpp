#pragma once

#include <string.h>
#include <map>
#include "api/sys/trace/Types.hpp"



namespace base::trace {

   class Logger
   {
      private:
         Logger( );
         Logger( const eLogStrategy&, const char* const );
         static Logger* mp_instance;
      public:
         ~Logger( );
         static Logger& instance( );
         static Logger& instance( const eLogStrategy&, const char* const );

         static bool init( const eLogStrategy&, const char* const );
         static bool deinit( );

         template< typename... Args >
         void message( const eLogLevel& log_level, const char* const format, Args... args )
         {
            char p_buffer[1024];
            auto size = ::sprintf( p_buffer, format, args... );

            switch( m_log_strategy )
            {
               case eLogStrategy::CONSOLE:
               case eLogStrategy::CONSOLE_EXT:
               {
                  ::write( STDOUT_FILENO, p_buffer, size );
                  break;
               }
               case eLogStrategy::DLT:
               {
                  #ifdef USE_DLT
                     DLT_LOG( dlt_context( ), to_dlt( log_level ), DLT_SIZED_CSTRING( p_buffer, size ) );
                  #endif // OS == USE_DLT
                  break;
               }
               case eLogStrategy::ANDROID:
               {
                  #if OS == OS_ANDROID
                     __android_log_print( to_android( log_level ), "TAG", "%s", p_buffer );
                  #endif // OS == OS_ANDROID
                  break;
               }
               default: break;
            }
         }

         template< typename... Args >
         void message_format(
                     const eLogLevel& log_level,
                     const char* const file, const char* const function, const int line,
                     const char* const format, Args... args
            )
         {
            const std::size_t full_format_max_size = 1024;

            switch( m_log_strategy )
            {
               case eLogStrategy::CONSOLE:
               {
                  char full_format[ full_format_max_size ] = { 0 };
                  strcat( full_format, PREFIX_FORMAT_MICROSECONDS_PID_TID_CODE );
                  strcat( full_format, to_color( log_level ) );
                  strcat( full_format, format );
                  strcat( full_format, "\n" );

                  message(
                        log_level, full_format,
                        time( eGranularity::microseconds ), getpid( ), pthread_self( ),
                        file, function, line,
                        args...
                     );
                  break;
               }
               case eLogStrategy::CONSOLE_EXT:
               {
                  tm* time_tm;
                  std::size_t milliseconds = 0;
                  local_time_of_date( time_tm, milliseconds );

                  char full_format[ full_format_max_size ] = { 0 };
                  strcat( full_format, PREFIX_FORMAT_DATE_TIME_MILLISECONDS_PID_TID_CODE );
                  strcat( full_format, to_color( log_level ) );
                  strcat( full_format, format );
                  strcat( full_format, "\n" );

                  message(
                        log_level, full_format,
                        time_tm->tm_year + 1900, time_tm->tm_mon + 1, time_tm->tm_mday,
                        time_tm->tm_hour, time_tm->tm_min, time_tm->tm_sec, milliseconds,
                        getpid( ), pthread_self( ),
                        file, function, line,
                        args...
                     );
                  break;
               }
               case eLogStrategy::DLT:
               {
                  char full_format[ full_format_max_size ] = { 0 };
                  strcat( full_format, "[%s:%s:%d] -> " );
                  strcat( full_format, format );
                  strcat( full_format, "\n" );

                  message( log_level, full_format, file, function, line, args... );
                  break;
               }
               case eLogStrategy::ANDROID:
               {
                  char full_format[ full_format_max_size ] = { 0 };
                  strcat( full_format, "[%s:%s:%d] -> " );
                  strcat( full_format, format );
                  strcat( full_format, "\n" );

                  message( log_level, full_format, file, function, line, args... );
                  break;
               }
               default: break;
            }
         }

         template< typename... Args >
         static void verbose( const char* const format, Args&... args )
         {
            instance( ).message( eLogLevel::VERBOSE, format, args... );
         }

         template< typename... Args >
         static void debug( const char* const format, Args&... args )
         {
            instance( ).message( eLogLevel::DEBUG, format, args... );
         }

         template< typename... Args >
         static void info( const char* const format, Args&... args )
         {
            instance( ).message( eLogLevel::INFO, format, args... );
         }

         template< typename... Args >
         static void warning( const char* const format, Args&... args )
         {
            instance( ).message( eLogLevel::WARNING, format, args... );
         }

         template< typename... Args >
         static void error( const char* const format, Args&... args )
         {
            instance( ).message( eLogLevel::ERROR, format, args... );
         }

         template< typename... Args >
         static void fatal( const char* const format, Args&... args )
         {
            instance( ).message( eLogLevel::FATAL, format, args... );
         }

      private:
         const char* mp_application_name = nullptr;

      public:
         const eLogStrategy& log_strategy( ) const;
         void log_strategy( const eLogStrategy& );
      private:
         #if OS == OS_ANDROID
            eLogStrategy m_log_strategy = eLogStrategy::ANDROID;
         #else
            eLogStrategy m_log_strategy = eLogStrategy::CONSOLE;
         #endif

      private:
         #ifdef USE_DLT
            DltContext& dlt_context( );
            std::map< pthread_t, DltContext > m_context_map;
         #endif
   };



   inline
   const eLogStrategy& Logger::log_strategy( ) const
   {
      return m_log_strategy;
   }

   inline
   void Logger::log_strategy( const eLogStrategy& _log_strategy )
   {
      m_log_strategy = _log_strategy;

      #ifndef USE_DLT
         // Prevent DLT logging startegy in case of DLT is not used
         if( eLogStrategy::DLT == m_log_strategy )
         {
            #if OS == OS_ANDROID
               m_log_strategy = eLogStrategy::ANDROID;
            #else
               m_log_strategy = eLogStrategy::CONSOLE;
            #endif
         }
      #endif

      #if OS != OS_ANDROID
         if( eLogStrategy::ANDROID == m_log_strategy )
            m_log_strategy = eLogStrategy::CONSOLE;
      #endif
   }

} // namespace base::trace



#define CLASS_ABBR "MAIN"

#define TRACE_LOG( LOG_LEVEL, USER_FORMAT, ... ) \
   do { \
      ::base::trace::Logger::instance( ).message_format( \
         LOG_LEVEL, \
         CLASS_ABBR, __FUNCTION__, __LINE__, \
         "" USER_FORMAT, ##__VA_ARGS__ \
      ); \
   } while( false )

#undef CLASS_ABBR
