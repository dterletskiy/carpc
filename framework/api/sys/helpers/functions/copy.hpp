#pragma once

#include <cstdlib>



namespace base {

   /****************************************************************************************************
    * 
    * Copy buffer function (instead of memcpy)
    * 
    ***************************************************************************************************/
   void copy( void* _p_destination, void* _p_source, const std::size_t _size );

} // namespace base
