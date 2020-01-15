#pragma once

#include <string>
#include <vector>
#include <sstream>
#include "api/sys/helpers/macros/arguments.hpp"



/****************************************************************************************************
 * 
 * Macro defines class enumeration without default values for items.
 * This macro also defines method to_string what converts enumeration to string value.
 * It is implemented as lambde to have a possibility to use this macro in functions, classes, headers.
 * !!! Default values for enumeration itemes should not be used !!!
 * link: http://qaru.site/questions/9343/is-there-a-simple-way-to-convert-c-enum-to-string
 * Examples:
 *    DEFINE_ENUM( eColors, size_t, _RED, _GREEN, _BLUE );
 *    std::cout << to_string( eColors::_RED ) << std::endl;
 * 
 ***************************************************************************************************/
#define DEFINE_ENUM( NAME, TYPE, ... ) \
   enum class NAME : TYPE \
   { \
      __VA_ARGS__, \
      _COUNT_, \
      _FIRST_ = __FIRST_ARG__( __VA_ARGS__ ), \
      _LAST_ = static_cast< TYPE >( __FIRST_ARG__( __VA_ARGS__ ) ) + _COUNT_ - 1 \
   }; \
auto to_string = []( NAME value ) -> const std::string \
{ \
   static std::string enum_name = #NAME "::"; \
   static std::vector< std::string > strings; \
   if( strings.empty( ) ) \
   { \
      static std::string str = #__VA_ARGS__; \
      static size_t len = str.length(); \
      std::ostringstream temp; \
      for( size_t i = 0; i < len; i++ ) \
      { \
         if( isspace( str[i] ) ) continue; \
         else if( str[i] == ',' ) \
         { \
            strings.push_back( temp.str() ); \
            temp.str( std::string() );\
         } \
         else temp << str[i]; \
      } \
      strings.push_back( temp.str() ); \
   } \
   return enum_name + strings[ static_cast< size_t >( value ) ]; \
};

/****************************************************************************************************
 * 
 * Macro defines class enumeration without default values for items.
 * This macro also defines method to_string what converts enumeration to string value.
 * It is implemented as lambde to have a possibility to use this macro in classes, headers.
 * !!! Default values for enumeration itemes should not be used !!!
 * This macro also generates additional methods what allow to iterate by enumeration values in for.
 * !!! This macro should be used outside of any function !!!
 * link: http://qaru.site/questions/9343/is-there-a-simple-way-to-convert-c-enum-to-string
 * Examples:
 *    DEFINE_ENUM( eColors, size_t, _RED, _GREEN, _BLUE );
 *    for( const auto& item : eColors() )
 *       std::cout << to_string( item ) << std::endl;
 * 
 ***************************************************************************************************/
#define DEFINE_ENUM_EX( NAME, TYPE, ... ) \
DEFINE_ENUM( NAME, TYPE, __VA_ARGS__ ); \
inline NAME operator++( NAME& x ) { return x = ( NAME )( static_cast< TYPE >(x) + 1 ); }; \
inline NAME operator*( NAME c ) { return c; }; \
inline NAME begin( NAME r ) { return NAME::_FIRST_; }; \
inline NAME end( NAME r ) { NAME l = NAME::_LAST_; return ++l; };
