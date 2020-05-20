#include <algorithm>

#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/tools/Tools.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TOOLS"



namespace base::tools::id {

   namespace { os::Mutex s_mutex; }
   const ID generate( const char* const name_space )
   {
      base::os::MutexAutoLocker locker( s_mutex );

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

} // namespace base::tools::id



namespace base::tools::cmd {

   char* get_option( int argc, char** begin, const std::string & option )
   {
      char** end = begin + argc;
      char** itr = std::find( begin, end, option );
      if( itr != end && ++itr != end )
         return *itr;

      return nullptr;
   }

   bool is_option_exists( int argc, char** begin, const std::string& option )
   {
      char** end = begin + argc;
      return std::find( begin, end, option ) != end;
   }



   using tCmdLineMap = std::map< std::string, std::string >;
   tCmdLineMap s_cmd_line_map;
   std::string s_delimiter = "=";

   void init( int argc, char** argv )
   {
      if( false == s_cmd_line_map.empty( ) )
         return;


      for( int i = 0; i < argc; ++i )
      {
         std::string argument( argv[ i ] );
         size_t position = argument.find( s_delimiter );
         if( std::string::npos == position )
            s_cmd_line_map.emplace( argument, std::string( "" ) );
         else
            s_cmd_line_map.emplace( argument.substr( 0, position ), argument.substr( position + 1 ) );
      }
   }

   tCmdParamValue argument( const std::string& parameter )
   {
      const auto iterator = s_cmd_line_map.find( parameter );
      if( s_cmd_line_map.end( ) == iterator )
         return std::nullopt;

      return iterator->second;
   }

} // namespace base::tools::cmd



#include <random>
#include <sstream>
#include "uuid/uuid.h"

// link: https://stackoverflow.com/a/60198074/12978480
namespace base::tools::uuid {

   static std::random_device                 rd;
   static std::mt19937                       gen( rd( ) );
   static std::uniform_int_distribution< >   dis( 0, 15 );
   static std::uniform_int_distribution< >   dis2( 8, 11 );

   void generate_random( std::string& _uuid )
   {
      std::stringstream ss;
      ss << std::hex;
      for( size_t i = 0; i < 8; i++ ) ss << dis( gen );
      ss << "-";
      for( size_t i = 0; i < 4; i++ ) ss << dis( gen );
      ss << "-4";
      for( size_t i = 0; i < 3; i++ ) ss << dis( gen );
      ss << "-";
      ss << dis2( gen );
      for( size_t i = 0; i < 3; i++ ) ss << dis( gen );
      ss << "-";
      for( size_t i = 0; i < 12; i++ ) ss << dis( gen );

      _uuid = ss.str( );
   }

}
