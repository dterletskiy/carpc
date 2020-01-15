#pragma once

#include "api/sys/common/Types.hpp"



/****************************************************************************************************
 * 
 * Macro returns amount of arguments
 * Returns the number of arguments contained in __VA_ARGS__ before expansion as far as NARG is >0 and <70
 * Parameters could be any sign
 * link: https://stackoverflow.com/questions/2124339/c-preprocessor-va-args-number-of-arguments
 * Examples:
 *    std::cout << ARGS_COUNT( 1, 2, 3, a ,"b" ) << std::endl;
 * 
 ***************************************************************************************************/
#ifdef _MSC_VER // Microsoft compilers
   #define INTERNAL_ARGS_AUGMENTER(...) DUMMY, __VA_ARGS__
   #define _GET_ARG_N_( \
          _1_,  _2_,  _3_,  _4_,  _5_,  _6_,  _7_,  _8_,  _9_, _10_, \
         _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, \
         _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, \
         _31_, _32_, _33_, _34_, _35_, _36_, _37_, _38_, _39_, _40_, \
         _41_, _42_, _43_, _44_, _45_, _46_, _47_, _48_, _49_, _50_, \
         _51_, _52_, _53_, _54_, _55_, _56_, _57_, _58_, _59_, _60_, \
         _61_, _62_, _63_, _64_, _65_, _66_, _67_, _68_, _69_, _70_, \
         count, ...) count
   #define _REVERSE_SEQUENCE_() \
      69, 68, 67, 66, 65, 64, 63, 62, 61, 60, \
      59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
      49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
      39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
      29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
      19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
       9,  8,  7,  6,  5,  4,  3,  2,  1,  0
#else // Non-Microsoft compilers
   #define INTERNAL_ARGS_AUGMENTER(...) DUMMY, ## __VA_ARGS__
   #define _GET_ARG_N_( \
          _0_, \
          _1_,  _2_,  _3_,  _4_,  _5_,  _6_,  _7_,  _8_,  _9_, _10_, \
         _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, \
         _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, \
         _31_, _32_, _33_, _34_, _35_, _36_, _37_, _38_, _39_, _40_, \
         _41_, _42_, _43_, _44_, _45_, _46_, _47_, _48_, _49_, _50_, \
         _51_, _52_, _53_, _54_, _55_, _56_, _57_, _58_, _59_, _60_, \
         _61_, _62_, _63_, _64_, _65_, _66_, _67_, _68_, _69_, _70_, \
         count, ...) count
   #define _REVERSE_SEQUENCE_() \
      70, \
      69, 68, 67, 66, 65, 64, 63, 62, 61, 60, \
      59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
      49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
      39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
      29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
      19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
       9,  8,  7,  6,  5,  4,  3,  2,  1,  0
#endif // Microsoft compilers
#define INTERNAL_EXPAND(x) x
#define INTERNAL_EXPAND_ARGS_PRIVATE(...) INTERNAL_EXPAND(_GET_ARG_N_(__VA_ARGS__))
#define ARGS_COUNT(...)  INTERNAL_EXPAND_ARGS_PRIVATE(INTERNAL_ARGS_AUGMENTER(__VA_ARGS__), _REVERSE_SEQUENCE_())



/****************************************************************************************************
 * 
 * Macro returns amount of arguments
 * Returns the number of arguments contained in __VA_ARGS__ before expansion as far as NARG is >0 and <70
 * Parameters could be only compilable sign such as variable, digit, char
 * link: https://stackoverflow.com/questions/2124339/c-preprocessor-va-args-number-of-arguments
 * Examples:
 *    std::cout << ARGS_COUNT_C( 1, 2, 3, "b" ) << std::endl;
 * 
 ***************************************************************************************************/
#define ARGS_COUNT_C(...) detail::args_count(__VA_ARGS__)
namespace detail
{
   template< typename ...Args >
   constexpr std::size_t args_count( Args&&... ) { return sizeof...(Args); }
}



/****************************************************************************************************
 * 
 * Examples:
 *    C_ASSERT(1 ==  COUNT_ARGS(1));      // OK
 *    C_ASSERT(1 ==  COUNT_ARGS(1, 2));   // ERROR
 * 
 ***************************************************************************************************/
#define C_ASSERT(test) switch(0) { case 0: case test:; }


/****************************************************************************************************
 * 
 * Macro returns first argument
 * link: https://stackoverflow.com/questions/4750688/how-to-single-out-the-first-parameter-sent-to-a-macro-taking-only-a-variadic-par
 * Examples:
 *    std::cout << GET_FIRST_ARG( 1, 2, 3, 4, 5 ) << std::endl;
 * 
 ***************************************************************************************************/
#define _FIRST_ARG_(N, ...) N
#define _FIRST_ARG(args) _FIRST_ARG_ args
#define GET_FIRST_ARG(...) _FIRST_ARG((__VA_ARGS__, 0))



/****************************************************************************************************
 * 
 * Check if parameters are empty.
 * link: https://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments/
 * Examples:
 *    std::cout << IS_EMPTY( ) << std::endl;
 *    std::cout << IS_EMPTY( 1, 2, 3 ) << std::endl;
 * 
 ***************************************************************************************************/
#define _ARG16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define HAS_COMMA(...) _ARG16(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define _TRIGGER_PARENTHESIS_(...) ,
 
#define IS_EMPTY(...)                                                                           \
_ISEMPTY_(                                                                                      \
      /* test if there is just one argument, eventually an empty one */                         \
      HAS_COMMA(__VA_ARGS__),                                                                   \
      /* test if _TRIGGER_PARENTHESIS_ together with the argument adds a comma */               \
      HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__),                                             \
      /* test if the argument together with a parenthesis adds a comma */                       \
      HAS_COMMA(__VA_ARGS__ (/*empty*/)),                                                       \
      /* test if placing it between _TRIGGER_PARENTHESIS_ and the parenthesis adds a comma */   \
      HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__ (/*empty*/))                                  \
   )

#define PASTE5(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4
#define _ISEMPTY_(_0, _1, _2, _3) HAS_COMMA(PASTE5(_IS_EMPTY_CASE_, _0, _1, _2, _3))
#define _IS_EMPTY_CASE_0001 ,



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






#define COMPARE_TYPES( TYPE_A, TYPE_B ) std::is_same_v< TYPE_A, TYPE_B >

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
