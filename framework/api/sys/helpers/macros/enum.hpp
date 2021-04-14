#pragma once

#include <string>
#include <vector>
#include <sstream>
#include "api/sys/helpers/macros/arguments.hpp"



/****************************************************************************************************
 * 
 * Macro defines class enumeration without default values for items.
 * This macro also defines method 'to_string' what converts enumeration to string value.
 * It is implemented as lambda to have a possibility to use this macro in functions, classes, headers.
 * !!! Default values for enumeration itemes should not be used !!!
 * link: http://qaru.site/questions/9343/is-there-a-simple-way-to-convert-c-enum-to-string
 * Examples:
 *    DEFINE_ENUM( eColors, std::size_t, _RED, _GREEN, _BLUE );
 *    std::cout << to_string( eColors::_RED ) << std::endl;
 * 
 ***************************************************************************************************/
// auto to_string = []( NAME value ) -> const std::string
#define DEFINE_ENUM( NAME, TYPE, ... ) \
   enum class NAME : TYPE \
   { \
      __VA_ARGS__, \
      _COUNT_, \
      _FIRST_ = __FIRST_ARG__( __VA_ARGS__ ), \
      _LAST_ = static_cast< TYPE >( __FIRST_ARG__( __VA_ARGS__ ) ) + _COUNT_ - 1, \
      _BEGIN_ = __FIRST_ARG__( __VA_ARGS__ ), \
      _END_ = static_cast< TYPE >( __FIRST_ARG__( __VA_ARGS__ ) ) + _COUNT_ \
   }; \
   inline const std::string to_string( const NAME value ) \
   { \
      static std::string enum_name = #NAME "::"; \
      static std::vector< std::string > strings; \
      if( strings.empty( ) ) \
      { \
         static std::string str = #__VA_ARGS__; \
         static std::size_t len = str.length(); \
         std::ostringstream temp; \
         for( std::size_t i = 0; i < len; i++ ) \
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
      if( strings.size( ) - 1 < static_cast< std::size_t >( value ) ) \
         return enum_name + std::string( "UNDEFINED" ); \
      else \
         return enum_name + strings[ static_cast< std::size_t >( value ) ]; \
   };

/****************************************************************************************************
 * 
 * Macro defines class enumeration without default values for items.
 * This macro also defines method 'to_string' what converts enumeration to string value.
 * It is implemented as lambda to have a possibility to use this macro in classes, headers.
 * !!! Default values for enumeration itemes should not be used !!!
 * This macro also generates additional methods what allow to iterate by enumeration values in for.
 * !!! This macro should be used outside of any function !!!
 * link: http://qaru.site/questions/9343/is-there-a-simple-way-to-convert-c-enum-to-string
 * Examples:
 *    DEFINE_ENUM( eColors, std::size_t, _RED, _GREEN, _BLUE );
 *    for( const auto& item : eColors() )
 *       std::cout << to_string( item ) << std::endl;
 * 
 ***************************************************************************************************/
#define DEFINE_ENUM_EX( NAME, TYPE, ... ) \
   DEFINE_ENUM( NAME, TYPE, __VA_ARGS__ ); \
   inline NAME operator++( NAME& x ) { return x = ( NAME )( static_cast< TYPE >(x) + 1 ); }; \
   inline NAME operator*( NAME c ) { return c; }; \
   inline NAME begin( NAME r ) { return NAME::_BEGIN_; }; \
   inline NAME end( NAME r ) { return NAME::_END_; };






/****************************************************************************************************
 * 
 * Special structure for "enum" and "class enum" substitution.
 * This structure has the same meaning as "class enum" but extended by member functions what allow to use it in:
 * "switch", "rage for", printing and other additional scopes where usual "enum" and "class enum" can't be used.
 * Value "UNDEFINED" will be added automatically.
 * !!! Default values for enumeration itemes should not be used !!!
 * 
 * Examples:
 * DEFINE_ENUMIRATION( eDigit, std::size_t, ONE, TWO, THREE )
 * int main( int argc, char** argv, char** envp )
 * {
 *    eDigit digit = eDigit::THREE;
 *    printf( "digit: %s", digit.to_string( ).c_str( ) );
 * 
 *    for( const auto& item : eDigit( ) )
 *       std::cout << item.to_string( ) << std::endl;
 * 
 *    return 0;
 * }
 * 
 ***************************************************************************************************/
#define DEFINE_ENUMIRATION( NAME, ENUM_TYPE, ... ) \
   struct NAME \
   { \
      using TYPE = ENUM_TYPE; \
      enum eValue : TYPE \
      { \
         __VA_ARGS__, UNDEFINED, \
         _COUNT_, \
         _FIRST_ = __FIRST_ARG__( __VA_ARGS__ ), \
         _LAST_ = static_cast< TYPE >( __FIRST_ARG__( __VA_ARGS__ ) ) + _COUNT_ - 1, \
         _BEGIN_ = __FIRST_ARG__( __VA_ARGS__ ), \
         _END_ = static_cast< TYPE >( __FIRST_ARG__( __VA_ARGS__ ) ) + _COUNT_ \
      }; \
 \
      NAME( ) = default; \
      constexpr NAME( eValue _value ) : value( _value ) { } \
 \
      operator eValue( ) const { return value; } \
 \
      explicit operator bool( ) const { return NAME::UNDEFINED != value; } \
 \
      constexpr bool operator==( const NAME& other ) const { return value == other.value; } \
 \
      constexpr bool operator!=( const NAME& other ) const { return value != other.value; } \
 \
      NAME& operator++( ) \
      { \
         value = ( eValue )( static_cast< TYPE >( value ) + 1 ); \
         return *this; \
      }; \
 \
      NAME& operator*( ) { return *this; }; \
 \
      NAME& begin( ) { static NAME first = NAME::_BEGIN_; return first; } \
 \
      NAME& end( ) { static NAME last = NAME::_END_; return last; } \
 \
      const std::string to_string( ) const \
      { \
         static std::string enum_name = #NAME "::"; \
         static std::vector< std::string > strings; \
         if( strings.empty( ) ) \
         { \
            static std::string str = #__VA_ARGS__; \
            static std::size_t len = str.length(); \
            std::ostringstream temp; \
            for( std::size_t i = 0; i < len; i++ ) \
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
         if( strings.size( ) - 1 < static_cast< std::size_t >( value ) ) \
            return enum_name + std::string( "UNDEFINED" ); \
         else \
            return enum_name + strings[ static_cast< std::size_t >( value ) ]; \
      }; \
 \
      eValue value = UNDEFINED; \
   };

