#pragma once

#include <string>
#include <sstream>
#include <optional>
#include <initializer_list>



namespace base {

   /****************************************************************************************************
    * 
    * Building formated string
    * 
    ***************************************************************************************************/

   const auto DEC = std::dec;
   const auto HEX = std::hex;
   const auto OCT = std::oct;

   template< typename ... TYPES >
   const std::string format_string( const TYPES& ... args )
   {
      std::ostringstream ss;
      (void)std::initializer_list< int >{ ( ss << args, 0 )... };
      return ss.str( );
   }

   template< typename TYPE >
   const std::string to_string( const std::optional< TYPE >& optional )
   {
      if( std::nullopt == optional )
         return "std::nullopt";
      return format_string( optional.value( ) );
   }

} // namespace base
