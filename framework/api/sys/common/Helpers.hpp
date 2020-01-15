#pragma once

#include "api/sys/common/Types.hpp"




/****************************************************************************************************
 * 
 * Function pointer helper macros
 * 
 * Examples:
 * 
 * class Consumer
 * {
 * public:
 *    void print( int value ) { DBG_INF( "%d", value ); }
 * };
 * void print( int value ) { DBG_INF( "%d", value ); }
 * void print( ) { DBG_INF( ); }

 * int main( int argc, char *argv[] )
 * {

 *    Consumer consumer;

 *    DEFINE_FUNCTION_POINTER( void, pf_void );
 *    SET_FUNCTION_POINTER( pf_void, print );
 *    CALL_FUNCTION_POINTER( pf_void );

 *    DEFINE_FUNCTION_POINTER( void, pf_int, int );
 *    SET_FUNCTION_POINTER( pf_int, print );
 *    CALL_FUNCTION_POINTER( pf_int, 123 );

 *    DEFINE_CLASS_FUNCTION_POINTER( void, Consumer, pf_class_int, int );
 *    SET_CLASS_FUNCTION_POINTER( pf_class_int, Consumer, print );
 *    CALL_CLASS_FUNCTION_POINTER( consumer, pf_class_int, 456 );

 *    DEFINE_FUNCTION_POINTER_TYPE( void, tpf_int, int );
 *    SET_FUNCTION_POINTER_TYPE( tpf_int, pf1, print );
 *    CALL_FUNCTION_POINTER( pf1, 123 );

 *    DEFINE_CLASS_FUNCTION_POINTER_TYPE( void, Consumer, tpf_class_int, int );
 *    SET_CLASS_FUNCTION_POINTER_TYPE( tpf_class_int, pf2, Consumer, print );
 *    CALL_CLASS_FUNCTION_POINTER( consumer, pf2, 456 );

 *    return 0;
 * }
 * 
 ***************************************************************************************************/
#define DEFINE_FUNCTION_POINTER_TYPE( RETURN_TYPE, TYPE, ... ) \
   typedef RETURN_TYPE ( *TYPE ) ( __VA_ARGS__ );
   // using TYPE = RETURN_TYPE (*)( __VA_ARGS__ );
#define DEFINE_FUNCTION_POINTER( RETURN_TYPE, NAME, ... ) \
   RETURN_TYPE ( *NAME )( __VA_ARGS__ );
#define SET_FUNCTION_POINTER_TYPE( TYPE, NAME, FUNCTION ) \
   TYPE NAME = &FUNCTION;
#define SET_FUNCTION_POINTER( NAME, FUNCTION ) \
   NAME = &FUNCTION;
#define CALL_FUNCTION_POINTER( NAME, ... ) \
   NAME( __VA_ARGS__ );

#define DEFINE_CLASS_FUNCTION_POINTER_TYPE( RETURN_TYPE, NS, TYPE, ... ) \
   typedef RETURN_TYPE ( NS::*TYPE ) ( __VA_ARGS__ );
   // using TYPE = RETURN_TYPE ( NS::* )( __VA_ARGS__ );
#define DEFINE_CLASS_FUNCTION_POINTER( RETURN_TYPE, NS, NAME, ... ) \
   RETURN_TYPE ( NS::*NAME )( __VA_ARGS__ );
#define SET_CLASS_FUNCTION_POINTER_TYPE( TYPE, NAME, NS, FUNCTION ) \
   TYPE NAME = &NS::FUNCTION;
#define SET_CLASS_FUNCTION_POINTER( NAME, NS, FUNCTION ) \
   NAME = &NS::FUNCTION;
#define CALL_CLASS_FUNCTION_POINTER( OBJECT, NAME, ... ) \
   ( OBJECT.*NAME )( __VA_ARGS__ );








#define DEAD *((unsigned int*)0) = 0xDEAD;

#define BOOL_TO_STRING( value ) ( value ? "true" : "false" )

#define CONCAT_STRINGS_2( STRING1, STRING2                                                ) STRING1 STRING2
#define CONCAT_STRINGS_3( STRING1, STRING2, STRING3                                       ) STRING1 STRING2 STRING3
#define CONCAT_STRINGS_4( STRING1, STRING2, STRING3, STRING4                              ) STRING1 STRING2 STRING3 STRING4
#define CONCAT_STRINGS_5( STRING1, STRING2, STRING3, STRING4, STRING5                     ) STRING1 STRING2 STRING3 STRING4 STRING5
#define CONCAT_STRINGS_6( STRING1, STRING2, STRING3, STRING4, STRING5, STRING6            ) STRING1 STRING2 STRING3 STRING4 STRING5 STRING6
#define CONCAT_STRINGS_7( STRING1, STRING2, STRING3, STRING4, STRING5, STRING6, STRING7   ) STRING1 STRING2 STRING3 STRING4 STRING5 STRING6 STRING7


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

namespace {
   using YES = char;
   struct NO { YES m[2]; };
}

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
 * Copy buffer function (instead of memcpy)
 * 
 ***************************************************************************************************/
void copy( void* _p_destination, void* _p_source, const size_t _size );



/****************************************************************************************************
 * 
 * Print std containers and types
 * 
 ***************************************************************************************************/
void print( const void* p_buffer, const size_t size, const bool is_new_line = false );
void print( const std::string& string, const bool is_new_line = false );

template< typename TYPE >
void print( const TYPE& value, const bool is_new_line = false )
{
   std::cout << value;

   if( is_new_line ) std::cout << std::endl;
}


template< typename TYPE >
void print( const std::list< TYPE >& list, const bool is_new_line = false )
{
   std::cout << "{ " ;
   for ( const auto& value : list )
   {
      print( value );
      std::cout << " ";
   }
   std::cout << "}";

   if( is_new_line ) std::cout << std::endl;
}

template< typename TYPE >
void print( const std::vector< TYPE >& vector, const bool is_new_line = false )
{
   std::cout << "{ " ;
   for ( const auto& value : vector )
   {
      print( value );
      std::cout << " ";
   }
   std::cout << "}";

   if( is_new_line ) std::cout << std::endl;
}

template< typename TYPE >
void print( const std::set< TYPE >& set, const bool is_new_line = false )
{
   std::cout << "{ " ;
   for ( const auto& value : set )
   {
      print( value );
      std::cout << " ";
   }
   std::cout << "}";

   if( is_new_line ) std::cout << std::endl;
}

template< typename TYPE_KEY, typename TYPE_VALUE >
void print( const std::pair< TYPE_KEY, TYPE_VALUE >& pair, const bool is_new_line = false )
{
   std::cout << "{ " ;
   print( pair.first );
   std::cout << " => ";
   print( pair.second );
   std::cout << " }";

   if( is_new_line ) std::cout << std::endl;
}

template< typename TYPE_KEY, typename TYPE_VALUE >
void print( const std::map< TYPE_KEY, TYPE_VALUE >& map, const bool is_new_line = false )
{
   std::cout << "{ " ;
   for ( const auto& pair : map )
   {
      print( pair );
      std::cout << " ";
   }
   std::cout << " }";

   if( is_new_line ) std::cout << std::endl;
}



} // namespace base
