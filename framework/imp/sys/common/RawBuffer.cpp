#include "api/sys/helpers/functions/print.hpp"
#include "api/sys/common/RawBuffer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "R_BUFFER"




using namespace base;



RawBuffer RawBuffer::create( const size_t size )
{
   RawBuffer buffer;
   buffer.alloc( size );
   return buffer;
}

void RawBuffer::destroy( RawBuffer& buffer )
{
   buffer.free( );
}

bool RawBuffer::alloc( size_t _size )
{
   if( nullptr != ptr )
   {
      SYS_WRN( "already allocated" );
      return false;
   }

   if( 0 == _size )
   {
      if( 0 == size )
      {
         SYS_WRN( "allocate 0 bytes" );
         return false;
      }
   }
   else
   {
      size = _size;
   }

   ptr = ::malloc( size );
   if( nullptr == ptr )
   {
      SYS_WRN( "allocate error" );
      size = 0;
      return false;
   }

   return true;
}

void RawBuffer::free( )
{
   if( nullptr != ptr )
      ::free( ptr );
   size = 0;
}

void RawBuffer::fill( const uint8_t symbol, const ssize_t delta ) const
{
   uint8_t* p = static_cast< uint8_t* >( ptr );
   for( size_t i = 0; i < size; ++i )
      p[ i ] = symbol + delta * i;
}

void RawBuffer::dump( ) const
{
   print( ptr, size );
}
