#include "api/sys/common/Includes.hpp"

#pragma once



namespace base {

   struct RawBuffer
   {
      static RawBuffer create( const size_t size );
      static void destroy( RawBuffer& buffer );

      bool alloc( size_t size = 0 );
      void free( );
      void fill( const uint8_t symbol, const ssize_t delta = 0 ) const;
      void dump( ) const;

      void* ptr = nullptr;
      size_t size = 0;
   };


} // namespace base
