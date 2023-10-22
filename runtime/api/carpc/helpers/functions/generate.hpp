#pragma once

#include <cstdlib>
#include <string>
#include <type_traits>
#include <functional>

#include "carpc/common/Definitions.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "HELPERS"



namespace carpc::generate {

   /***************************************************************************
    * 
    * Function for genetating string name based on template type parameter.
    * In case of allowed RTTI by build system "typeid( ... )" will be used.
    * In case of RTTI is not allowed by build system "__PRETTY_FUNCTION__"
    * macro will be used.
    * 
    **************************************************************************/
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

   /***************************************************************************
    * 
    * Function for genetating hash code based on template type parameter.
    * In case of allowed RTTI by build system "typeid( ... )" will be used.
    * In case of RTTI is not allowed by build system "std::hash" will be used.
    * 
    **************************************************************************/
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



namespace carpc::generate::random {

   void init( );
   std::string text( const std::size_t length );
   std::size_t number( const std::size_t begin, const std::size_t end );

}



#undef CLASS_ABBR
