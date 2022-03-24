#pragma once

#include <string>
#include <type_traits>
#include <functional>

#include "api/sys/common/Definitions.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "HELPERS"



namespace carpc::generate {

   /****************************************************************************************************
    * 
    * Function for genetating
    * 
    ***************************************************************************************************/
   template< typename T >
   std::string name( )
   {
      std::string value{ };
      #ifdef RTTI_ENABLED
         value = typeid( T ).name( );
      #else
         value = __PRETTY_FUNCTION__;
      #endif
      // SYS_ERR( "%s", value.c_str( ) );
      return value;
   }

   template< typename T >
   std::size_t hash( )
   {
      std::size_t value{ };
      #ifdef RTTI_ENABLED
         value = typeid( T ).hash_code( );
      #else
         value = std::hash< std::string >{ }( std::string{ __PRETTY_FUNCTION__ } );
      #endif
      // SYS_ERR( "%zu", value );
      return value;
   }

} // namespace carpc::generate



#undef CLASS_ABBR
