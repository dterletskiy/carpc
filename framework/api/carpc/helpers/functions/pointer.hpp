#pragma once

#include <cstdlib>
#include <cstdint>
#include <cstddef>



namespace carpc {

   void* const inc( void* const ptr, const std::size_t value );
   const void* const inc( const void* const ptr, const std::size_t value );

   void* const dec( void* const ptr, const std::size_t value );
   const void* const dec( const void* const ptr, const std::size_t value );

   std::ptrdiff_t diff( const void* const ptr1, const void* const ptr2 );

} // namespace carpc
