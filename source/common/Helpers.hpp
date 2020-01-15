#include "Types.hpp"

#pragma once



namespace base {


/****************************************************************************************************
 * 
 * This checks is class B is the base for class D
 * Is D inherited from B
 * Usage:
 *    
 *    class A { };
 *    class B : public A { };
 *    class C : public B { };
 *    class D { };
 *    int main( )
 *    {
 *       printf( "A is %s base for B", base::is_base_of< A, B >::value ? "" : "not" );
 *       printf( "A is %s base for C", base::is_base_of< A, C >::value ? "" : "not" );
 *       printf( "A is %s base for D", base::is_base_of< A, D >::value ? "" : "not" );
 *    
 *       return 0;
 *    }
 * 
 ***************************************************************************************************/

using YES = char;
struct NO { YES m[2]; };

template< typename B, typename D >
struct is_base_of
{
   static YES test( B* );
   static NO test( ... );

   static bool const value = sizeof( YES ) == sizeof( test( (D*)0 ) );
};

template< typename D >
struct is_base_of< D, D >
{
   static bool const value = false;
};



/**************************************************
 * 
 *************************************************/
template< typename T >
const std::string to_string( const T& data )
{
   if constexpr( std::is_integral< T >::value )
   {
      // return itoa( data );

      // return std::to_string( data ).c_str( );

      std::stringstream ss;
      ss << data;
      return ss.str( );
   }
   else if constexpr( std::is_same< T, std::string >::value )
   {
      return data;
   }

   return { "NoName" };
}



} // namespace base
