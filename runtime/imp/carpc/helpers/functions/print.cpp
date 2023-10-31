#include <bitset>
#include "carpc/helpers/functions/print.hpp"



namespace carpc {

   void print_hex( const void* const p_buffer, const std::size_t size, const bool is_new_line )
   {
      for( std::size_t i = 0; i < size; ++i )
         printf( "%#x ", static_cast< const std::uint8_t* const >( p_buffer )[i] );

      if( is_new_line ) printf( "\n" );
   }

   void print_bin( const void* const p_buffer, const std::size_t size, const bool is_new_line )
   {
      static const char* const nibble_representation[16] = {
         [ 0] = "0000", [ 1] = "0001", [ 2] = "0010", [ 3] = "0011",
         [ 4] = "0100", [ 5] = "0101", [ 6] = "0110", [ 7] = "0111",
         [ 8] = "1000", [ 9] = "1001", [10] = "1010", [11] = "1011",
         [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
      };

      const std::uint8_t* p_byte = static_cast< const std::uint8_t* >( p_buffer );
      for( std::size_t i = 0; i < size; ++i, ++p_byte )
      {
         std::uint8_t byte = *p_byte;
         printf( "%s%s", nibble_representation[ byte >> 4 ], nibble_representation[ byte & 0x0F ] );
      }

      if( is_new_line ) printf( "\n" );
   }

   template< >
   void print( const std::basic_string< char >& string, const bool is_new_line )
   {
      printf( "\"%s\"", string.c_str( ) );

      if( is_new_line ) printf( "\n" );
   }

   template< >
   void print( const std::basic_string< wchar_t >& string, const bool is_new_line )
   {
      printf( "\"%ls\"", string.c_str( ) );

      if( is_new_line ) printf( "\n" );
   }

} // namespace carpc
