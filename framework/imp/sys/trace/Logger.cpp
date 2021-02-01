#include "api/sys/helpers/functions/format.hpp"
#include "api/sys/trace/Logger.hpp"



namespace {

   std::size_t NAMELEN = 16;

}



using namespace base::trace;



Logger* Logger::mp_instance = nullptr;

Logger::Logger( const eLogStrategy& log_strategy, const char* const app_name )
   : mp_application_name( app_name )
   , m_log_strategy( log_strategy )
{
   #if OS == OF_LINUX
      const auto registerAppStatus = dlt_register_app( mp_application_name, format_string( mp_application_name, " Application" ).c_str( ) );
      if( DLT_RETURN_OK != registerAppStatus )
         return;
   #endif
}

Logger::~Logger( )
{
   #if OS == OF_LINUX
      for( auto& pair : m_context_map )
         dlt_unregister_context( &(pair.second) );
      dlt_unregister_app( );
   #endif
}

#if OS == OF_LINUX
DltContext& Logger::dlt_context( )
{
   const auto [iterator, success] = m_context_map.insert( { pthread_self( ), DltContext{ } } );
   if( true == success )
   {
      static thread_local char* const thread_name = new char [ NAMELEN ];
      if ( 0 != pthread_getname_np( iterator->first, thread_name, NAMELEN ) )
      {
         std::string thread_name_id = format_string( iterator->first ).c_str( );
         dlt_register_context( &(iterator->second), thread_name_id.c_str( ), format_string( thread_name_id, " Context" ).c_str( ) );
      }
      else
      {
         dlt_register_context( &(iterator->second), thread_name, format_string( thread_name, " Context" ).c_str( ) );
      }
   }

   return iterator->second;
}
#endif

Logger& Logger::instance( )
{
   if( nullptr == mp_instance )
   {
      static Logger dummy_logger;
      return dummy_logger;
   }

   return *mp_instance;
}

Logger& Logger::instance( const eLogStrategy& log_strategy, const char* const app_name )
{
   if( nullptr == mp_instance )
      mp_instance = new Logger( log_strategy, app_name );

   return *mp_instance;
}

bool Logger::init( const eLogStrategy& log_strategy, const char* const app_name )
{
   instance( log_strategy, app_name );

   return true;
}

bool Logger::deinit( )
{
   delete mp_instance;
   return true;
}
