#include "api/sys/helpers/functions/print.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "HELPERS"



namespace base {

   void print( const void* const p_buffer, const size_t size, const bool is_new_line )
   {
      for( size_t i = 0; i < size; ++i )
         printf( "%#x ", static_cast< const std::uint8_t* const >( p_buffer )[i] );

      if( is_new_line ) printf( "\n" );
   }

   void print( const std::string& string, const bool is_new_line )
   {
      printf( "\"%s\"", string.c_str( ) );

      if( is_new_line ) printf( "\n" );
   }

} // namespace base
