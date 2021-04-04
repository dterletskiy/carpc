#pragma once

#include "api/sys/helpers/functions/is_base.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IS_DEFINED"



/****************************************************************************************************
 * 
 * This functionality allows to check if some method is defined for some object in compile time
 * and call it in case if it exusts.
 * 
 ***************************************************************************************************/
namespace base::is_defined::__private__ {

   template< typename RV >
   struct Result
   {
      bool is_defined = false;
      std::optional< RV > value = std::nullopt;
   };

   template< >
   struct Result< void >
   {
      bool is_defined = false;
   };

} // namespace base::is_defined::__private__



#define DEFINE_CALL_IF_EXISTS( METHOD_NAME )                                                                                           \
   namespace base::is_defined {                                                                                                        \
                                                                                                                                       \
      template< typename RV, typename... Args >                                                                                        \
      struct METHOD_NAME {                                                                                                             \
         public:                                                                                                                       \
            using tResult = __private__::Result< RV >;                                                                                 \
                                                                                                                                       \
         private:                                                                                                                      \
            enum class eAction : std::uint8_t { CALL, CHECK };                                                                         \
                                                                                                                                       \
            template< typename T >                                                                                                     \
            struct Helper                                                                                                              \
            {                                                                                                                          \
               template< typename Z, RV (Z::*)( Args... ) = &Z::METHOD_NAME >                                                          \
               struct wrapper { };                                                                                                     \
                                                                                                                                       \
               template< typename C >                                                                                                  \
               static base::YES check( wrapper< C >* );                                                                                \
                                                                                                                                       \
               template< typename C >                                                                                                  \
               static base::NO check( ... );                                                                                           \
                                                                                                                                       \
               static bool const value = sizeof( base::YES ) == sizeof( check< T >( 0 ) );                                             \
                                                                                                                                       \
               static void debug( )                                                                                                    \
               {                                                                                                                       \
                  SYS_DBG( "%s", typeid( Helper ).name( ) );                                                                           \
               }                                                                                                                       \
            };                                                                                                                         \
                                                                                                                                       \
         private:                                                                                                                      \
            template< typename T >                                                                                                     \
            static tResult caller( base::YES, T& object, Args... args )                                                                \
            {                                                                                                                          \
               if constexpr( true == std::is_same_v< void, RV > )                                                                      \
               {                                                                                                                       \
                  object.METHOD_NAME( std::forward< Args >( args )... );                                                               \
                  return { true };                                                                                                     \
               }                                                                                                                       \
               else                                                                                                                    \
               {                                                                                                                       \
                  RV value = object.METHOD_NAME( std::forward< Args >( args )... );                                                    \
                  return { true, value };                                                                                              \
               }                                                                                                                       \
            }                                                                                                                          \
                                                                                                                                       \
            template< typename T >                                                                                                     \
            static tResult caller( base::NO, T& object, Args... args )                                                                 \
            {                                                                                                                          \
               return { };                                                                                                             \
            }                                                                                                                          \
                                                                                                                                       \
         public:                                                                                                                       \
            template< typename T >                                                                                                     \
            static tResult call( T& object, Args... args )                                                                             \
            {                                                                                                                          \
               if( 0 ) Helper< T >::debug( );                                                                                          \
               using DEFINED = typename base::Bool2Type< Helper< T >::value >::type;                                                   \
               return caller( DEFINED( ), object, args... );                                                                           \
            }                                                                                                                          \
                                                                                                                                       \
            template< typename T >                                                                                                     \
            static bool check( T& object )                                                                                             \
            {                                                                                                                          \
               if( 0 ) Helper< T >::debug( );                                                                                          \
               return Helper< T >::value;                                                                                              \
            }                                                                                                                          \
      };                                                                                                                               \
                                                                                                                                       \
   } // namespace base::is_defined

#define DEFINE_CALL_IF_EXISTS_ALIAS( METHOD_NAME, ALIAS, RETURN_TYPE, ... )                                                            \
   namespace base::is_defined { using ALIAS = METHOD_NAME< RETURN_TYPE, ##__VA_ARGS__ >; }



#undef CLASS_ABBR



#if 0 // Example

   struct HasFoo
   {
      void foo( )
      {
         MSG_WRN( "void foo( )" );
      }

      void foo( std::size_t value )
      {
         MSG_WRN( "void foo( std::size_t )" );
         // return value + 1;
      }

      void foo( double& value )
      {
         MSG_WRN( "void foo( double& )" );
         value = 0;
      }

      void foo( int& value )
      {
         MSG_WRN( "void foo( int& )" );
      }

      int foo( std::size_t& value1, std::size_t value2 )
      {
         MSG_WRN( "void foo( std::size_t&, std::size_t )" );
         return value1 + value2;
      }
   };

   struct HasNoFoo
   {
      void goo( )
      {
         MSG_WRN( "void goo( )" );
      }   
   };



   DEFINE_CALL_IF_EXISTS( foo );
   DEFINE_CALL_IF_EXISTS_ALIAS( foo, foo_alias_0, void );



   int main( int argc, char** argv, char** envp )
   {
      HasFoo hf;
      HasNoFoo hnf;

      {
         base::is_defined::foo_alias_0::call( hf );
         base::is_defined::foo_alias_0::call( hnf );
      }

      {
         std::size_t value = 123;
         base::is_defined::foo< void, std::size_t >::call( hf, value );
      }

      {
         std::size_t value = 123;
         auto result_hf = base::is_defined::foo< int, std::size_t&, std::size_t >::call( hf, value, value );
         auto result_hnf = base::is_defined::foo< int, std::size_t&, std::size_t >::call( hnf, value, value );

         MSG_WRN( "is_defined = %d / returned value = %d", result_hf.is_defined, result_hf.value.value_or( 1234567890 ) );
         MSG_WRN( "is_defined = %d / returned value = %d", result_hnf.is_defined, result_hnf.value.value_or( 1234567890 ) );
      }

      {
         double value = 123.456;
         base::is_defined::foo< void, double& >::call( hf, value );
         MSG_WRN( "value = %f", value );
      }

      return 0;
   }

#endif






#if 0 // Example and depricated implementation

   #define DEFINE_CALL_IF_EXISTS( METHOD_NAME )                                                                                           \
      namespace base::is_defined {                                                                                                        \
                                                                                                                                          \
         struct METHOD_NAME {                                                                                                             \
            private:                                                                                                                      \
               enum class eAction : std::uint8_t { CALL, CHECK };                                                                         \
                                                                                                                                          \
               template< typename T, typename... Args >                                                                                   \
               struct Helper                                                                                                              \
               {                                                                                                                          \
                  template< typename Z, void (Z::*)( Args... ) = &Z::METHOD_NAME >                                                        \
                  struct wrapper { };                                                                                                     \
                                                                                                                                          \
                  template< typename C >                                                                                                  \
                  static base::YES check( wrapper< C >* );                                                                                \
                                                                                                                                          \
                  template< typename C >                                                                                                  \
                  static base::NO check( ... );                                                                                           \
                                                                                                                                          \
                  static bool const value = sizeof( base::YES ) == sizeof( check< T >( 0 ) );                                             \
                                                                                                                                          \
                  static void debug( )                                                                                                    \
                  {                                                                                                                       \
                     SYS_DBG( "%s", typeid( Helper ).name( ) );                                                                           \
                  }                                                                                                                       \
               };                                                                                                                         \
                                                                                                                                          \
            private:                                                                                                                      \
               template< typename T, typename... Args >                                                                                   \
               static bool caller( T& object, base::YES, Args&&... args )                                                                 \
               {                                                                                                                          \
                  object.METHOD_NAME( std::forward< Args >( args )... );                                                                  \
                  return true;                                                                                                            \
               }                                                                                                                          \
                                                                                                                                          \
               template< typename T, typename... Args >                                                                                   \
               static bool caller( T& object, base::NO, Args&&... args )                                                                  \
               {                                                                                                                          \
                  return false;                                                                                                           \
               }                                                                                                                          \
                                                                                                                                          \
            private:                                                                                                                      \
               template< typename T, typename... Args >                                                                                   \
               static bool checker( T& object, base::YES, Args&&... args )                                                                \
               {                                                                                                                          \
                  return true;                                                                                                            \
               }                                                                                                                          \
                                                                                                                                          \
               template< typename T, typename... Args >                                                                                   \
               static bool checker( T& object, base::NO, Args&&... args )                                                                 \
               {                                                                                                                          \
                  return false;                                                                                                           \
               }                                                                                                                          \
                                                                                                                                          \
            private:                                                                                                                      \
               template< typename T, typename... Args >                                                                                   \
               static bool check_or_call( const eAction& action, T& object, Args&&... args )                                              \
               {                                                                                                                          \
                  constexpr bool has_0 = Helper< T, Args... >::value;                                                                     \
                  constexpr bool has_1 = Helper< T, typename std::remove_reference< Args& >::type... >::value;                            \
                  constexpr bool has_2 = Helper< T, typename std::remove_reference< Args&& >::type... >::value;                           \
                  constexpr bool has_3 = Helper< T, typename std::add_lvalue_reference< Args& >::type... >::value;                        \
                  constexpr bool has_4 = Helper< T, typename std::add_lvalue_reference< Args&& >::type... >::value;                       \
                  constexpr bool has_5 = Helper< T, typename std::add_rvalue_reference< Args& >::type... >::value;                        \
                  constexpr bool has_6 = Helper< T, typename std::add_rvalue_reference< Args&& >::type... >::value;                       \
                  using DEFINED = typename base::Bool2Type< has_0 || has_1 || has_2 || has_3 || has_4 || has_5 || has_6 >::type;          \
                                                                                                                                          \
                  if( 0 ) Helper< T, Args... >::debug( );                                                                                 \
                                                                                                                                          \
                  switch( action )                                                                                                        \
                  {                                                                                                                       \
                     case eAction::CALL:  return caller( object, DEFINED( ), args... );                                                   \
                     case eAction::CHECK: return checker( object, DEFINED( ), args... );                                                  \
                  }                                                                                                                       \
                  SYS_ERR( "Undefined action" );                                                                                          \
                  return false;                                                                                                           \
               }                                                                                                                          \
                                                                                                                                          \
            public:                                                                                                                       \
               template< typename T, typename... Args >                                                                                   \
               static bool call( T& object, Args&&... args )                                                                              \
               {                                                                                                                          \
                  return check_or_call( eAction::CALL, object, args... );                                                                 \
               }                                                                                                                          \
                                                                                                                                          \
               template< typename T, typename... Args >                                                                                   \
               static bool check( T& object, Args&&... args )                                                                             \
               {                                                                                                                          \
                  return check_or_call( eAction::CHECK, object, args... );                                                                \
               }                                                                                                                          \
         };                                                                                                                               \
                                                                                                                                          \
      } // namespace base::is_defined



   struct HasFoo
   {
      void foo( )
      {
         MSG_WRN( "void foo( )" );
      }

      void foo( std::size_t value )
      {
         MSG_WRN( "void foo( std::size_t )" );
      }

      void foo( double& value )
      {
         MSG_WRN( "void foo( double& )" );
         value = 0;
      }

      void foo( int& value )
      {
         MSG_WRN( "void foo( int& )" );
      }

      void foo( std::string&& value )
      {
         MSG_WRN( "void foo( std::string&& )" );
      }
   };

   struct HasNoFoo
   {
      void goo( )
      {
         MSG_WRN( "void goo( )" );
      }   
   };

   DEFINE_CALL_IF_EXISTS( foo );

   int main( int argc, char** argv, char** envp )
   {
      HasFoo hf;
      HasNoFoo hnf;

      {
         MSG_MARKER( "BEGIN" );
         base::is_defined::foo::call( hf );
         base::is_defined::foo::call( hnf );
         MSG_MARKER( "END" );
      }
      printf("\n");

      {
         MSG_MARKER( "BEGIN" );
         std::size_t value = 123;
         base::is_defined::foo::call( hf, value );
         base::is_defined::foo::call( hnf, value );
         MSG_MARKER( "END" );
      }
      printf("\n");

      {
         MSG_MARKER( "BEGIN" );
         base::is_defined::foo::call( hf, (std::size_t)123 );
         base::is_defined::foo::call( hnf, (std::size_t)123 );
         MSG_MARKER( "END" );
      }
      printf("\n");

      {
         MSG_MARKER( "BEGIN" );
         double value = 123.456;
         MSG_WRN( "value = %f", value );
         base::is_defined::foo::call( hf, value );
         base::is_defined::foo::call( hnf, value );
         MSG_WRN( "value = %f", value );
         MSG_MARKER( "END" );
      }
      printf("\n");

      {
         MSG_MARKER( "BEGIN" );
         base::is_defined::foo::call( hf, (int)123 );
         base::is_defined::foo::call( hnf, (int)123 );
         MSG_MARKER( "END" );
      }
      printf("\n");

      {
         MSG_MARKER( "BEGIN" );
         std::string value( "string" );
         base::is_defined::foo::call( hf, value );
         base::is_defined::foo::call( hnf, value );
         MSG_MARKER( "END" );
      }
      printf("\n");

      {
         MSG_MARKER( "BEGIN" );
         // base::is_defined::foo::call( hf, std::string( "string" ) );
         // base::is_defined::foo::call( hnf, std::string( "string" ) );
         MSG_MARKER( "END" );
      }
      printf("\n");

      return 0;
   }

#endif
