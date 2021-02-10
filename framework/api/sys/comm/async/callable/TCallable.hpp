#pragma once

#include <tuple>
#include "api/sys/comm/async/callable/ICallable.hpp"
// #include "api/sys/helpers/functions/print.hpp"



// Switching off compiler optimization to avoid warning
// warning: ‘<anonymous>’ is used uninitialized in this function [-Wuninitialized]
// which leads not saving paramers because of optimization in case of using function with
// parameters passed by reference.
// WARNING: Functions with parameters passed by reference or pointers must be used very carefyle because
// in current object will be samed corresponding refecences and pointers to these parameters, but these
// parameters could be chnaged after this and defore calling this function via this object (example 8).
#pragma GCC push_options
#pragma GCC optimize ("O0")

namespace base::async::callable {

   template < typename ...Args >
   struct TCallable : public ICallable
   {
      public:
         using tFunction = void (*)( Args... );

      public:
         TCallable( tFunction p_func, std::tuple< Args... > params )
            : mp_func( p_func )
            , m_params( params )
         {
         }

         TCallable( tFunction p_func, Args&... args )
            : mp_func( p_func )
            , m_params( args... )
         {
            // print_pack( args... );
         }

         TCallable( const TCallable& other )
            : mp_func( other.mp_func )
            , m_params( other.m_params )
         {
         }

         TCallable& operator=( const TCallable& other )
         {
            if( this == other )
               return *this;

            mp_func = other.mp_func;
            m_params = other.m_params;

            return *this;
         }

      public:
         static TCallable* const create( tFunction p_func, std::tuple< Args... > params )
         {
            return new TCallable( p_func, params );
         }

         void call( ) const
         {
            return call_function( typename gen_sequence< sizeof...( Args ) >::type( ) );
         }

      private:
         template< int ...S >
         void call_function( sequence< S... > ) const
         {
            // print_pack( std::get< S >( m_params )... );
            return mp_func( std::get< S >( m_params )... );
         }

      private:
         const tFunction mp_func;
         std::tuple< Args... > m_params;
   };

} // namespace base::async::callable

#pragma GCC pop_options



#if 0 // Example

   void function_0( )
   {
      SYS_DBG( );
   }

   void function_1( int id )
   {
      SYS_DBG( "id: %d", id );
   }

   void function_2( const int& id )
   {
      SYS_DBG( "id: %d", id );
   }

   void function_3( const std::size_t& id, const std::string& name )
   {
      MSG_DBG( "id: %zu / name: %s", id, name.c_str( ) );
   }



   int main( int argc, char* argv[ ] )
   {
      namespace base_v = base::async::callable;

      // Example 1
      {
         base_v::TCallable< > callable( function_0 );
         callable.call( );
      }

      // Example 2
      {
         std::tuple< int > parameters = std::make_tuple( 111 );
         base_v::TCallable< int > callable( function_1, parameters );
         callable.call( );
      }

      // Example 3
      {
         base_v::TCallable< int > callable( function_1, 111 );
         callable.call( );
      }

      // Example 4
      {
         const base_v::ICallable* const callable = base_v::TCallable< int >::create( function_1, 111 );
         callable->call( );
      }

      // Example 5
      {
         std::tuple< int > parameters = std::make_tuple( 222 );
         base_v::TCallable< const int& > callable( function_2, parameters );
         callable.call( );
      }

      // Example 6
      {
         base_v::TCallable< const int& > callable( function_2, 222 );
         callable.call( );
      }

      // Example 7
      {
         base_v::TCallable< const std::size_t&, const std::string& > callable( function_3, 333, "framework" );
         callable.call( );
      }

      // Example 8
      {
         std::size_t id = 111;
         std::string name = "framework";
         base_v::TCallable< const std::size_t&, const std::string& > callable( function_3, id, name );
         callable.call( );
         id = 222;
         name = "framework_1";
         callable.call( );
      }



      return 0;
   }

#endif
