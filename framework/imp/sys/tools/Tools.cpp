#include <algorithm>

#include "api/sys/common/Helpers.hpp"
#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/tools/Tools.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TOOLS"



namespace base::tools {


const ID generate_id( const char* const name_space )
{
   os::Mutex mutex( true );

   static std::map< std::string, ID > id_map;
   ID id = 1;
   auto iterator = id_map.find( name_space );
   if( iterator == id_map.end( ) )
   {
      id_map.emplace( std::make_pair( name_space, id ) );
   }
   else
   {
      id = ++( iterator->second );
   }

   return id;
}

char* get_cmd_option( int argc, char ** begin, const std::string & option )
{
   char** end = begin + argc;
   char** itr = std::find( begin, end, option );
   if( itr != end && ++itr != end )
      return *itr;

   return nullptr;
}

bool is_cmd_option_exists( int argc, char** begin, const std::string& option )
{
   char** end = begin + argc;
   return std::find( begin, end, option ) != end;
}



} // namespace base::tools
