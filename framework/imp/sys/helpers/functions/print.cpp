#include "api/sys/helpers/functions/print.hpp"



namespace base {

   void print( const void* p_buffer, const size_t size, const bool is_new_line )
   {
      for( size_t i = 0; i < size; ++i )
         printf( "%#x ", static_cast< const uint8_t* >( p_buffer )[i] );

      if( is_new_line ) std::cout << std::endl;
   }

   void print( const std::string& string, const bool is_new_line )
   {
      std::cout << "\"" << string << "\"";

      if( is_new_line ) std::cout << std::endl;
   }

} // namespace base
