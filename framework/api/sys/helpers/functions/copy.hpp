#pragma once

#include "api/sys/common/Types.hpp"



namespace base {

   /****************************************************************************************************
    * 
    * Copy buffer function (instead of memcpy)
    * 
    ***************************************************************************************************/
   void copy( void* _p_destination, void* _p_source, const size_t _size );

} // namespace base
