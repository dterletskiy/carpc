#include "api/sys/helpers/functions/print.hpp"
#include "api/sys/common/Types.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "COMMON"




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

bool RawBuffer::alloc( const size_t _size )
{
   if( 0 == _size )
   {
      SYS_WRN( "allocate 0 bytes" );
      return false;
   }

   if( nullptr != ptr )
   {
      SYS_WRN( "already allocated" );
      return false;
   }

   ptr = ::malloc( _size );
   if( nullptr == ptr )
   {
      SYS_WRN( "allocate error" );
      return false;
   }

   size = _size;
   return true;
}

void RawBuffer::free( )
{
   if( nullptr != ptr )
      ::free( ptr );
   size = 0;
}

void RawBuffer::dump( ) const
{
   print( ptr, size );
}
