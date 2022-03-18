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
// parameters could be chnaged after this and defore calling this function via this object (example 7).
#pragma GCC push_options
#pragma GCC optimize ("O0")

namespace carpc::async {

   template < typename ...Args >
   struct TCallable : public ICallable
   {
      public:
         using tFunctionPtr = void (*)( Args... );
         using tFunction = std::function< void( Args... ) >;
         using tParameters = std::tuple< Args... >;

      private:
         TCallable( tFunctionPtr p_func, tParameters params )
            : mp_function( p_func )
            , m_params( params )
         {
         }

         TCallable( tFunctionPtr p_func, Args&... args )
            : mp_function( p_func )
            , m_params( args... )
         {
         }

         TCallable( tFunction p_func, tParameters params )
            : m_function( p_func )
            , m_params( params )
         {
         }

         TCallable( tFunction p_func, Args&... args )
            : m_function( p_func )
            , m_params( args... )
         {
         }

         TCallable( const TCallable& ) = delete;
         TCallable& operator=( const TCallable& ) = delete;

      public:
         ~TCallable( ) override = default;

         static tSptr const create( tFunctionPtr p_func, tParameters params )
         {
            return std::shared_ptr< TCallable >( new TCallable( p_func, params ) );
         }

         static tSptr const create( tFunctionPtr p_func, Args&... args )
         {
            return std::shared_ptr< TCallable >( new TCallable( p_func, args...  ));
         }

         static tSptr const create( tFunction func, tParameters params )
         {
            return std::shared_ptr< TCallable >( new TCallable( func, params ) );
         }

         static tSptr const create( tFunction func, Args&... args )
         {
            return std::shared_ptr< TCallable >( new TCallable( func, args...  ));
         }

      public:
         void call( ) const
         {
            return call_function( typename gen_sequence< sizeof...( Args ) >::type( ) );
         }

      private:
         template< int ...S >
         void call_function( sequence< S... > ) const
         {
            // print_pack( std::get< S >( m_params )... );
            if( nullptr != mp_function )
               return mp_function( std::get< S >( m_params )... );
            else if( m_function )
               return m_function( std::get< S >( m_params )... );
         }

      private:
         const tFunctionPtr      mp_function = nullptr;
         const tFunction         m_function;
         const tParameters       m_params;
   };

} // namespace carpc::async

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
      namespace carpc_v = carpc::async;

      // Example 1
      {
         auto callable = base_v::TCallable< >::create( function_0 );
         callable->call( );
      }

      // Example 2
      {
         std::tuple< int > parameters = std::make_tuple( 111 );
         auto callable = base_v::TCallable< int >::create( function_1, parameters );
         callable->call( );
      }

      // Example 3
      {
         auto callable = base_v::TCallable< int >::create( function_1, 111 );
         callable->call( );
      }

      // Example 4
      {
         std::tuple< int > parameters = std::make_tuple( 222 );
         auto callable = base_v::TCallable< const int& >::create( function_2, parameters );
         callable.call( );
      }

      // Example 5
      {
         auto callable = base_v::TCallable< const int& >::create( function_2, 222 );
         callable.call( );
      }

      // Example 6
      {
         auto callable = base_v::TCallable< const std::size_t&, const std::string& >::create( function_3, 333, "framework" );
         callable.call( );
      }

      // Example 7
      {
         std::size_t id = 111;
         std::string name = "framework";
         auto callable = base_v::TCallable< const std::size_t&, const std::string& >::create( function_3, id, name );
         callable.call( );
         id = 222;
         name = "framework_1";
         callable.call( );
      }



      return 0;
   }

#endif
