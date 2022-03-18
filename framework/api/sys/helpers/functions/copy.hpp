#pragma once

#include <cstdlib>



namespace carpc {

   /****************************************************************************************************
    * 
    * Copy buffer function (instead of memcpy)
    * 
    ***************************************************************************************************/
   void copy( void* _p_destination, void* _p_source, const std::size_t _size );

} // namespace carpc
