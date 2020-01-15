#pragma once

#include "api/sys/common/Types.hpp"



#define DEAD *((unsigned int*)0) = 0xDEAD;



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



/****************************************************************************************************
 * 
 * Print std containers and types
 * 
 ***************************************************************************************************/

template< typename TYPE >
void print( const TYPE& value )
{
   std::cout << value;
}

template< typename TYPE >
void print( const std::list< TYPE >& list )
{
   std::cout << "{ " ;
   for ( const auto& value : list )
   {
      print( value );
      std::cout << " ";
   }
   std::cout << "}";
}

template< typename TYPE >
void print( const std::vector< TYPE >& vector )
{
   std::cout << "{ " ;
   for ( const auto& value : vector )
   {
      print( value );
      std::cout << " ";
   }
   std::cout << "}";
}

template< typename TYPE >
void print( const std::set< TYPE >& set )
{
   std::cout << "{ " ;
   for ( const auto& value : set )
   {
      print( value );
      std::cout << " ";
   }
   std::cout << "}";
}

template< typename TYPE_KEY, typename TYPE_VALUE >
void print( const std::pair< TYPE_KEY, TYPE_VALUE >& pair )
{
   std::cout << "{ " ;
   print( pair.first );
   std::cout << " => ";
   print( pair.second );
   std::cout << " }";
}

template< typename TYPE_KEY, typename TYPE_VALUE >
void print( const std::map< TYPE_KEY, TYPE_VALUE >& map )
{
   std::cout << "{ " ;
   for ( const auto& pair : map )
   {
      print( pair );
      std::cout << " ";
   }
   std::cout << " }";
}



} // namespace base
