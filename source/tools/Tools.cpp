#include "common/Helpers.hpp"
#include "oswrappers/Mutex.hpp"
#include "Tools.hpp"

#include "Trace.hpp"
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

   SYS_TRC( "%s", base::to_string( id ).c_str( ) );
   return id;
}



} // namespace base::tools
