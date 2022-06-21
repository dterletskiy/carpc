#include "carpc/helpers/functions/copy.hpp"



namespace carpc {

   /****************************************************************************************************
    * 
    * Copy buffer function (instead of memcpy)
    * 
    ***************************************************************************************************/
   void copy( void* _p_destination, void* _p_source, const std::size_t _size )
   {
      using type = std::size_t;

      std::size_t type_iterations = _size / sizeof( type );
      type* p_destination_type = static_cast< type* >( _p_destination );
      type* p_source_type = static_cast< type* >( _p_source );
      while( type_iterations-- ) *p_destination_type++ = *p_source_type++;

      std::size_t char_iterations = _size % sizeof( type );
      type* p_destination_char = static_cast< type* >( p_destination_type );
      type* p_source_char = static_cast< type* >( p_source_type );
      while( char_iterations-- ) *p_destination_char++ = *p_source_char++;
   }

} // namespace carpc
