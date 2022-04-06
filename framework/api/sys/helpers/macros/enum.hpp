#pragma once

#include <string>
#include <vector>
#include <sstream>
#include "api/sys/common/IPC.hpp"
#include "api/sys/helpers/macros/arguments.hpp"



/****************************************************************************************************
 * 
 * Macro defines class enumeration without default values for items.
 * This macro also defines method 'c_str' what converts enumeration to c-string value.
 * It is implemented as lambda to have a possibility to use this macro in functions, classes, headers.
 * !!! Default values for enumeration itemes should not be used !!!
 * link: http://qaru.site/questions/9343/is-there-a-simple-way-to-convert-c-enum-to-string
 * Examples:
 *    DEFINE_ENUM( eColors, std::size_t, _RED, _GREEN, _BLUE );
 *    std::cout << c_str( eColors::_RED ) << std::endl;
 * 
 ***************************************************************************************************/
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
   inline const char* const c_str( const NAME value ) \
   { \
      static std::string enum_name = #NAME "::"; \
      static std::vector< std::string > strings; \
      static const char* const undefined = #NAME "::UNDEFINED"; \
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
         return undefined; \
      else \
         return strings[ static_cast< std::size_t >( value ) ].c_str( ); \
   };

/****************************************************************************************************
 * 
 * Macro defines class enumeration without default values for items.
 * This macro also defines method 'c_str' what converts enumeration to c-string value.
 * It is implemented as lambda to have a possibility to use this macro in classes, headers.
 * !!! Default values for enumeration itemes should not be used !!!
 * This macro also generates additional methods what allow to iterate by enumeration values in for.
 * !!! This macro should be used outside of any function !!!
 * link: http://qaru.site/questions/9343/is-there-a-simple-way-to-convert-c-enum-to-string
 * Examples:
 *    DEFINE_ENUM( eColors, std::size_t, _RED, _GREEN, _BLUE );
 *    for( const auto& item : eColors() )
 *       std::cout << c_str( item ) << std::endl;
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
 * DEFINE_ENUMERATION( eDigit, std::size_t, ONE, TWO, THREE )
 * int main( int argc, char** argv, char** envp )
 * {
 *    eDigit digit = eDigit::THREE;
 *    printf( "digit: %s", digit.c_str( ) );
 * 
 *    for( const auto& item : eDigit( ) )
 *       std::cout << item.c_str( ) << std::endl;
 * 
 *    return 0;
 * }
 * 
 ***************************************************************************************************/
#define DEFINE_ENUMERATION( NAME, ENUM_TYPE, ... )                                           \
   struct NAME                                                                               \
   {                                                                                         \
      using TYPE = ENUM_TYPE;                                                                \
      enum eValue : TYPE                                                                     \
      {                                                                                      \
         __VA_ARGS__, UNDEFINED,                                                             \
         _COUNT_,                                                                            \
         _FIRST_ = __FIRST_ARG__( __VA_ARGS__ ),                                             \
         _LAST_ = static_cast< TYPE >( __FIRST_ARG__( __VA_ARGS__ ) ) + _COUNT_ - 1,         \
         _BEGIN_ = __FIRST_ARG__( __VA_ARGS__ ),                                             \
         _END_ = static_cast< TYPE >( __FIRST_ARG__( __VA_ARGS__ ) ) + _COUNT_               \
      };                                                                                     \
                                                                                             \
      NAME( ) = default;                                                                     \
      constexpr NAME( eValue _value ) : value( _value ) { }                                  \
                                                                                             \
      operator eValue( ) const { return value; }                                             \
      explicit operator bool( ) const { return NAME::UNDEFINED != value; }                   \
                                                                                             \
      constexpr bool operator==( const NAME& other ) const { return value == other.value; }  \
      constexpr bool operator!=( const NAME& other ) const { return value != other.value; }  \
      constexpr bool operator<=( const NAME& other ) const { return value <= other.value; }  \
      constexpr bool operator>=( const NAME& other ) const { return value >= other.value; }  \
      constexpr bool operator<( const NAME& other ) const { return value < other.value; }    \
      constexpr bool operator>( const NAME& other ) const { return value > other.value; }    \
                                                                                             \
      NAME& operator++( )                                                                    \
      {                                                                                      \
         value = ( eValue )( static_cast< TYPE >( value ) + 1 );                             \
         return *this;                                                                       \
      };                                                                                     \
      NAME& operator--( )                                                                    \
      {                                                                                      \
         value = ( eValue )( static_cast< TYPE >( value ) - 1 );                             \
         return *this;                                                                       \
      };                                                                                     \
                                                                                             \
      NAME& operator*( ) { return *this; };                                                  \
                                                                                             \
      NAME& begin( ) { static NAME _begin = NAME::_BEGIN_; return _begin; }                  \
      NAME& end( ) { static NAME _end = NAME::_END_; return _end; }                          \
                                                                                             \
      const char* const c_str( ) const                                                       \
      {                                                                                      \
         static std::string enum_name = #NAME "::";                                          \
         static std::string arguments = #__VA_ARGS__;                                        \
         static std::vector< std::string > strings;                                          \
         if( strings.empty( ) )                                                              \
         {                                                                                   \
            std::string argument;                                                            \
            for( std::string::size_type i = 0; i < arguments.length( ); i++ )                \
            {                                                                                \
               if( isspace( arguments[i] ) ) continue;                                       \
               else if( arguments[i] == ',' )                                                \
               {                                                                             \
                  strings.push_back( enum_name + argument );                                 \
                  argument.clear( );                                                         \
               }                                                                             \
               else argument.push_back( arguments[i] );                                      \
            }                                                                                \
            strings.push_back( enum_name + argument );                                       \
         }                                                                                   \
                                                                                             \
         if( strings.size( ) - 1 < static_cast< std::string::size_type >( value ) )          \
            return #NAME "::UNDEFINED";                                                      \
         else                                                                                \
            return strings[ static_cast< std::string::size_type >( value ) ].c_str( );       \
      };                                                                                     \
                                                                                             \
      bool to_stream( carpc::ipc::tStream& stream ) const                                    \
      {                                                                                      \
         return carpc::ipc::serialize( stream, value );                                      \
      }                                                                                      \
      bool from_stream( carpc::ipc::tStream& stream )                                        \
      {                                                                                      \
         return carpc::ipc::deserialize( stream, value );                                    \
      }                                                                                      \
                                                                                             \
      eValue value = UNDEFINED;                                                              \
   };

