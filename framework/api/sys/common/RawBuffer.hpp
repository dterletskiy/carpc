#pragma once

#include <cstdlib>
#include <cstdint>



namespace base {

   struct RawBuffer
   {
      static RawBuffer create( const std::size_t size );
      static void destroy( RawBuffer& buffer );

      bool alloc( std::size_t size = 0 );
      void free( );
      void fill( const std::uint8_t symbol, const ssize_t delta = 0 ) const;
      void dump( ) const;

      void* ptr = nullptr;
      std::size_t size = 0;
   };


} // namespace base
