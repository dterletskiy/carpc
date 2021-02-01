#pragma once



/****************************************************************************************************
 * 
 * Function pointer helper macros
 * 
 * Examples:
 * 
 * class Consumer
 * {
 * public:
 *    void print( int value ) { MSG_INF( "%d", value ); }
 * };
 * void print( int value ) { MSG_INF( "%d", value ); }
 * void print( ) { MSG_INF( ); }

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
