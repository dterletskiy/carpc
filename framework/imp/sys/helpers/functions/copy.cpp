#include "api/sys/helpers/functions/copy.hpp"



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

} // namespace base
