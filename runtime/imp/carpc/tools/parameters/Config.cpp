#include <fstream>
#include <filesystem>
#include <algorithm>

#include "carpc/helpers/functions/format.hpp"
#include "carpc/tools/parameters/Config.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "CONFIG"



using namespace carpc::parameters;



const tValue Config::default_prefix{ config::default_prefix };
const tValue Config::default_delimiter{ config::default_delimiter };



Config::Config( const tMap& map )
   : Base( map, "CONFIG" )
{
}

Config::tSptr Config::create( const std::string& file, const tMap& default_parameters )
{
   std::ifstream file_stream;
   file_stream.open( file.c_str( ) );
   // std::cout << file_stream.rdbuf( ) << std::endl;

   if( false == file_stream.is_open( ) )
   {
      MSG_WRN( "configuration file '%s' can't be found", file.c_str( ) );
      return nullptr;
   }

   bool result = true;
   tMap map;
   while( file_stream )
   {
      std::string line;
      std::getline( file_stream, line, '\n' );

      line = carpc::trim( line );
      if( true == line.empty( ) )
         continue;
      if( '#' == line[0] )
         continue;

      // Check parameter prefix
      if( 0 != line.rfind( Config::default_prefix, 0 ) )
      {
         result = false;
         MSG_ERR( "Invalid argument: '%s'", line.c_str( ) );
      }

      if( false == result )
         continue;

      // Remove parameter prefix for further processing
      line.erase( 0, default_prefix.length( ) );

      std::size_t position = line.find( default_delimiter );
      if( std::string::npos == position )
         map.emplace( line, invalid_value );
      else
         map.emplace( line.substr( 0, position ), line.substr( position + 1 ) );
   }

   file_stream.close( );

   for( auto& pair: default_parameters )
   {
      map.emplace( pair.first, pair.second );
   }

   struct make_shared_enabler : public Config
   {
      make_shared_enabler( const tMap& map )
         : Config( map )
      { }
   };

   return result ? std::make_shared< make_shared_enabler >( map ) : nullptr;
}
