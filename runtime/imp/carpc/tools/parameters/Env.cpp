#include <fstream>
#include <filesystem>
#include <algorithm>

#include "carpc/helpers/functions/format.hpp"
#include "carpc/tools/parameters/Env.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "ENV"



using namespace carpc::parameters;



const tValue Env::default_prefix{ env::default_prefix };
const tValue Env::default_delimiter{ env::default_delimiter };



Env::Env( const tMap& map )
   : Base( map, "ENV" )
{
}

Env::tSptr Env::create( char** envp, const tMap& default_parameters )
{
   if( nullptr == envp )
      return nullptr;

   bool result = true;
   tMap map;
   for( int count = 0; nullptr != envp[ count ]; ++count )
   {
      std::string argument( envp[ count ] );

      // Check parameter prefix
      if( 0 != argument.rfind( Env::default_prefix, 0 ) )
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

   struct make_shared_enabler : public Env
   {
      make_shared_enabler( const tMap& map )
         : Env( map )
      { }
   };

   return result ? std::make_shared< make_shared_enabler >( map ) : nullptr;
}
