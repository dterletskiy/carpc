#include <fstream>
#include <filesystem>
#include <algorithm>

#include "carpc/helpers/functions/format.hpp"
#include "carpc/tools/parameters/CmdLine.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "CMDLINE"



using namespace carpc::parameters;



const tValue CmdLine::default_prefix{ cmdline::default_prefix };
const tValue CmdLine::default_delimiter{ cmdline::default_delimiter };



CmdLine::CmdLine( const tMap& map )
   : Base( map, "CMDLINE" )
{
}

CmdLine::tSptr CmdLine::create( int argc, char** argv, const tMap& default_parameters )
{
   if( nullptr == argv )
      return nullptr;

   bool result = true;
   tMap map;
   for( int count = 1; count < argc; ++count )
   {
      std::string argument( argv[ count ] );

      // Check parameter prefix
      if( 0 != argument.rfind( CmdLine::default_prefix, 0 ) )
      {
         result = false;
         MSG_ERR( "Invalid argument: '%s'", argument.c_str( ) );
      }

      if( false == result )
         continue;

      // Remove parameter prefix for further processing
      argument.erase( 0, default_prefix.length( ) );

      std::size_t position = argument.find( default_delimiter );
      if( std::string::npos == position )
         map.emplace( argument, invalid_value );
      else
         map.emplace( argument.substr( 0, position ), argument.substr( position + 1 ) );
   }

   for( auto& pair: default_parameters )
   {
      map.emplace( pair.first, pair.second );
   }

   struct make_shared_enabler : public CmdLine
   {
      make_shared_enabler( const tMap& map )
         : CmdLine( map )
      { }
   };

   return result ? std::make_shared< make_shared_enabler >( map ) : nullptr;
}
