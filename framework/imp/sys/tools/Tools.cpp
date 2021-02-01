#include "api/sys/tools/Tools.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TOOLS"



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

   const tCmdLineMap& map( )
   {
      return s_cmd_line_map;
   }

   void print( )
   {
      MSG_INF( "Command line parameters:" );
      for( auto pair : s_cmd_line_map )
      {
         MSG_VRB( "   %s = %s", pair.first.c_str( ), pair.second.c_str( ) );
      }
   }

} // namespace base::tools::cmd



namespace base::tools::cfg {

   tCfgLineMap s_cfg_line_map;
   std::string s_delimiter = "=";
   std::string s_file;

   void init( const std::string& file )
   {
      if( false == s_cfg_line_map.empty( ) )
         return;

      s_file = file;

      std::ifstream file_stream;
      file_stream.open( s_file.c_str( ) );
      // std::cout << file_stream.rdbuf( ) << std::endl;

      if( false == file_stream.is_open( ) )
      {
         MSG_WRN( "configuration file '%s' can't be found", s_file.c_str( ) );
         return;
      }

      while( file_stream )
      {
         std::string line;
         std::getline( file_stream, line, '\n' );

         if( true == line.empty( ) )
            continue;

         size_t position = line.find( s_delimiter );
         if( std::string::npos == position )
            s_cfg_line_map.emplace( line, std::string("") );
         else
            s_cfg_line_map.emplace( line.substr( 0, position ), line.substr( position + 1 ) );
      }

      file_stream.close( );
   }

   tCfgParamValue argument( const std::string& parameter )
   {
      const auto iterator = s_cfg_line_map.find( parameter );
      if( s_cfg_line_map.end( ) == iterator )
         return std::nullopt;

      return iterator->second;
   }

   const tCfgLineMap& map( )
   {
      return s_cfg_line_map;
   }

   void print( )
   {
      MSG_INF( "Configuration file parameters (%s):", s_file.c_str( ) );
      for( auto pair : s_cfg_line_map )
      {
         MSG_VRB( "   %s = %s", pair.first.c_str( ), pair.second.c_str( ) );
      }
   }

} // namespace base::tools::cfg



#include <random>
#include <sstream>
// #include "uuid/uuid.h"

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
