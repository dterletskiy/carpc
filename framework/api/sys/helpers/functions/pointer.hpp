#include "api/sys/common/Includes.hpp"

#pragma once



namespace base {

   void* const inc( void* const ptr, const size_t value );
   const void* const inc( const void* const ptr, const size_t value );

   void* const dec( void* const ptr, const size_t value );
   const void* const dec( const void* const ptr, const size_t value );

   ptrdiff_t diff( const void* const ptr1, const void* const ptr2 );

} // namespace base
