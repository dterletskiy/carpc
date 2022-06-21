#include <bitset>
#include "carpc/helpers/functions/print.hpp"



namespace carpc {

   // void print( const void* const p_buffer, const std::size_t size, const bool is_new_line )
   // {
   //    for( std::size_t i = 0; i < size; ++i )
   //       printf( "%#x ", static_cast< const std::uint8_t* const >( p_buffer )[i] );

   //    if( is_new_line ) printf( "\n" );
   // }

   void print_hex( const void* const p_buffer, const std::size_t size, const bool is_new_line )
   {
      for( std::size_t i = 0; i < size; ++i )
         printf( "%#x ", static_cast< const std::uint8_t* const >( p_buffer )[i] );

      if( is_new_line ) printf( "\n" );
   }

   void print_bin( const void* const p_buffer, const std::size_t size, const bool is_new_line )
   {
      for( std::size_t i = 0; i < size; ++i )
      {
         std::bitset< 8 > x( static_cast< const std::uint8_t* const >( p_buffer )[i] );
         std::cout << x << " ";
      }

      if( is_new_line ) printf( "\n" );
   }

   void print( const std::string& string, const bool is_new_line )
   {
      printf( "\"%s\"", string.c_str( ) );

      if( is_new_line ) printf( "\n" );
   }

} // namespace carpc
