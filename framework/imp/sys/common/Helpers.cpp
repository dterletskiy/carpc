#include "api/sys/common/Helpers.hpp"



namespace base {


/****************************************************************************************************
 * 
 * Copy buffer function (instead of memcpy)
 * 
 ***************************************************************************************************/
void copy( void* _p_destination, void* _p_source, const size_t _size )
{
   using type = size_t;

   size_t type_iterations = _size / sizeof( type );
   type* p_destination_type = static_cast< type* >( _p_destination );
   type* p_source_type = static_cast< type* >( _p_source );
   while( type_iterations-- ) *p_destination_type++ = *p_source_type++;

   size_t char_iterations = _size % sizeof( type );
   type* p_destination_char = static_cast< type* >( p_destination_type );
   type* p_source_char = static_cast< type* >( p_source_type );
   while( char_iterations-- ) *p_destination_char++ = *p_source_char++;
}



/****************************************************************************************************
 * 
 * Print std containers and types
 * 
 ***************************************************************************************************/
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
