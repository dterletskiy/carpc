#pragma once

#include "api/sys/common/Includes.hpp"



namespace base {

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

      if( is_new_line ) printf( "\n" );
   }

   // template< typename TYPE >
   // void print( const TYPE* pointer, const bool is_new_line = false )
   // {
   //    printf( "%#x", pointer );

   //    if( is_new_line ) printf( "\n" );
   // }


   template< typename TYPE >
   void print( const TYPE* array, const size_t size, const bool is_new_line = false )
   {
      printf( "{ " );
      for ( size_t index = 0; index < size; ++index )
      {
         print( array[ index ] );
         printf( " " );
      }
      printf( "}" );

      if( is_new_line ) printf( "\n" );
   }


   template< typename TYPE >
   void print( const std::list< TYPE >& list, const bool is_new_line = false )
   {
      printf( "{ " );
      for ( const auto& value : list )
      {
         print( value );
         printf( " " );
      }
      printf( "}" );

      if( is_new_line ) printf( "\n" );
   }

   template< typename TYPE >
   void print( const std::vector< TYPE >& vector, const bool is_new_line = false )
   {
      printf( "{ " );
      for ( const auto& value : vector )
      {
         print( value );
         printf( " " );
      }
      printf( "}" );

      if( is_new_line ) printf( "\n" );
   }

   template< typename TYPE >
   void print( const std::set< TYPE >& set, const bool is_new_line = false )
   {
      printf( "{ " );
      for ( const auto& value : set )
      {
         print( value );
         printf( " " );
      }
      printf( "}" );

      if( is_new_line ) printf( "\n" );
   }

   template< typename TYPE_KEY, typename TYPE_VALUE >
   void print( const std::pair< TYPE_KEY, TYPE_VALUE >& pair, const bool is_new_line = false )
   {
      printf( "{ " );
      print( pair.first );
      printf( " => " );
      print( pair.second );
      printf( " }" );

      if( is_new_line ) printf( "\n" );
   }

   template< typename TYPE_KEY, typename TYPE_VALUE >
   void print( const std::map< TYPE_KEY, TYPE_VALUE >& map, const bool is_new_line = false )
   {
      printf( "{ " );
      for ( const auto& pair : map )
      {
         print( pair );
         printf( " " );
      }
      printf( "}" );

      if( is_new_line ) printf( "\n" );
   }

   inline
   void print_pack( )
   {
      std::cout << std::endl;
   }

   template< typename T, typename...Ts >
   void print_pack( T&& first, Ts&&... rest )
   {
      print( std::forward< T >( first ) );
      printf( " " );
      print_pack( std::forward< Ts >( rest )... );
   }

} // namespace base
