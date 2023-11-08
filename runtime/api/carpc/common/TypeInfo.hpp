#pragma once

/**************************************************
 * 
 * Defined types:
 * std::int8_t, std::int16_t, std::int32_t, std::int64_t
 * std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t
 * 
 * ***********************************************/
#include <cstdint>

/**************************************************
 * 
 * Defined types:
 * std::size_t, std::ptrdiff_t, std::nullptr_t,
 * std::max_align_t, std::byte
 *
 * ***********************************************/
#include <cstddef>

#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <stack>
#include <list>
#include <forward_list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <tuple>

#include <optional>

#include <memory>

#include "carpc/helpers/macros/strings.hpp"
#include "carpc/helpers/macros/types.hpp"



namespace carpc::types {

   using tID = std::uint8_t;
   const tID invalid_id = std::numeric_limits< tID >::max( );



   template< typename T >
   struct TTypeInfo
   {
      using TYPE = T;

      static constexpr const char* const name = "undefined";
      static constexpr tID id = invalid_id;

      const char* const get_name( ) const { return name; }
      tID get_id( ) const                 { return id;   }
   };



   #define DEFINE_TYPE_INFO_SIMPLE( ID, T )                       \
      template< >                                                 \
      struct TTypeInfo< T >                                       \
      {                                                           \
         using TYPE = T;                                          \
         static constexpr const char* const name = #T;            \
         static constexpr tID id = ID;                            \
                                                                  \
         const char* const get_name( ) const { return name; }     \
         tID get_id( ) const                 { return id;   }     \
      };

   #define DEFINE_TYPE_INFO_TEMPLATE( ID, T )                     \
      template< typename ... TYPES >                              \
      struct TTypeInfo< T< TYPES... > >                           \
      {                                                           \
         using TYPE = T< TYPES... >;                              \
         static constexpr const char* const name = #T;            \
         static constexpr tID id = ID;                            \
                                                                  \
         const char* const get_name( ) const { return name; }     \
         tID get_id( ) const                 { return id;   }     \
      };




   DEFINE_TYPE_INFO_SIMPLE(  1, std::uint8_t );
   DEFINE_TYPE_INFO_SIMPLE(  2, std::uint16_t );
   DEFINE_TYPE_INFO_SIMPLE(  3, std::uint32_t );
   DEFINE_TYPE_INFO_SIMPLE(  4, std::uint64_t );

   DEFINE_TYPE_INFO_SIMPLE(  5, std::int8_t );
   DEFINE_TYPE_INFO_SIMPLE(  6, std::int16_t );
   DEFINE_TYPE_INFO_SIMPLE(  7, std::int32_t );
   DEFINE_TYPE_INFO_SIMPLE(  8, std::int64_t );

   DEFINE_TYPE_INFO_SIMPLE(  9, char );
   DEFINE_TYPE_INFO_SIMPLE( 10, bool );

   DEFINE_TYPE_INFO_SIMPLE( 11, float );
   DEFINE_TYPE_INFO_SIMPLE( 12, double );
   DEFINE_TYPE_INFO_SIMPLE( 13, long double );

   DEFINE_TYPE_INFO_TEMPLATE( 15, std::basic_string );
   DEFINE_TYPE_INFO_TEMPLATE( 16, std::vector );
   DEFINE_TYPE_INFO_TEMPLATE( 17, std::list );
   DEFINE_TYPE_INFO_TEMPLATE( 18, std::forward_list );
   DEFINE_TYPE_INFO_TEMPLATE( 19, std::queue );
   DEFINE_TYPE_INFO_TEMPLATE( 20, std::deque );
   DEFINE_TYPE_INFO_TEMPLATE( 21, std::stack );
   DEFINE_TYPE_INFO_TEMPLATE( 22, std::set );
   DEFINE_TYPE_INFO_TEMPLATE( 23, std::multiset );
   DEFINE_TYPE_INFO_TEMPLATE( 24, std::unordered_set );
   DEFINE_TYPE_INFO_TEMPLATE( 25, std::unordered_multiset );
   DEFINE_TYPE_INFO_TEMPLATE( 26, std::pair );
   DEFINE_TYPE_INFO_TEMPLATE( 27, std::map );
   DEFINE_TYPE_INFO_TEMPLATE( 28, std::multimap );
   DEFINE_TYPE_INFO_TEMPLATE( 29, std::unordered_map );
   DEFINE_TYPE_INFO_TEMPLATE( 30, std::unordered_multimap );
   DEFINE_TYPE_INFO_TEMPLATE( 31, std::tuple );

}



#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "TYPE_INFO"

#define PRINT_COMPARE_TYPES( A, B )                      \
   SYS_INF( "'%s' %s '%s'",                              \
         #A,                                             \
         CARPC_IS_SAME_TYPES( A, B ) ? "==" : "!=",      \
         #B                                              \
      );

#define PRINT_TYPE_INFO( ... )                           \
   SYS_INF( "type: %s / name: %s / id: %u",              \
         #__VA_ARGS__,                                   \
         carpc::types::TTypeInfo< __VA_ARGS__ >::name,   \
         carpc::types::TTypeInfo< __VA_ARGS__ >::id      \
      );

#undef CLASS_ABBR



#if 0

int main( int argc, char** argv, char** envp )
{
   PRINT_TYPE_INFO( void* );
   PRINT_TYPE_INFO( bool );
   PRINT_TYPE_INFO( char );

   PRINT_TYPE_INFO( unsigned char );
   PRINT_TYPE_INFO( unsigned short int );
   PRINT_TYPE_INFO( unsigned int );
   PRINT_TYPE_INFO( unsigned long int );
   PRINT_TYPE_INFO( unsigned long long int );

   PRINT_TYPE_INFO( signed char );
   PRINT_TYPE_INFO( signed short int );
   PRINT_TYPE_INFO( signed int );
   PRINT_TYPE_INFO( signed long int );
   PRINT_TYPE_INFO( signed long long int );

   PRINT_TYPE_INFO( float );
   PRINT_TYPE_INFO( double );
   PRINT_TYPE_INFO( long double );

   PRINT_TYPE_INFO( std::uint8_t );
   PRINT_TYPE_INFO( std::uint16_t );
   PRINT_TYPE_INFO( std::uint32_t );
   PRINT_TYPE_INFO( std::uint64_t );

   PRINT_TYPE_INFO( std::int8_t );
   PRINT_TYPE_INFO( std::int16_t );
   PRINT_TYPE_INFO( std::int32_t );
   PRINT_TYPE_INFO( std::int64_t );


   PRINT_TYPE_INFO( std::string );
   PRINT_TYPE_INFO( std::vector< std::uint8_t > );
   PRINT_TYPE_INFO( std::list< std::uint8_t > );
   PRINT_TYPE_INFO( std::forward_list< std::uint8_t > );
   PRINT_TYPE_INFO( std::queue< std::uint8_t > );
   PRINT_TYPE_INFO( std::deque< std::uint8_t > );
   PRINT_TYPE_INFO( std::stack< std::uint8_t > );
   PRINT_TYPE_INFO( std::set< std::uint8_t > );
   PRINT_TYPE_INFO( std::multiset< std::uint8_t > );
   PRINT_TYPE_INFO( std::unordered_set< std::uint8_t > );
   PRINT_TYPE_INFO( std::unordered_multiset< std::uint8_t > );
   PRINT_TYPE_INFO( std::pair< std::uint8_t, std::string > );
   PRINT_TYPE_INFO( std::map< std::uint8_t, std::string > );
   PRINT_TYPE_INFO( std::multimap< std::uint8_t, std::string > );
   PRINT_TYPE_INFO( std::unordered_map< std::uint8_t, std::string > );
   PRINT_TYPE_INFO( std::unordered_multimap< std::uint8_t, std::string > );
   PRINT_TYPE_INFO( std::tuple< std::uint8_t, std::int64_t, std::string > );

   return 0;
}

#endif
