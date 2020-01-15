#include <string.h>
#include <fcntl.h>
#include <execinfo.h>

#include "api/sys/oswrappers/linux/kernel.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Kernel"



namespace base::os::linux {



int set_nonblock( int fd )
{
   int flags = -1;
#if defined( O_NONBLOCK )
   if( -1 == ( flags = fcntl( fd, F_GETFL, 0 ) ) )
      flags = 0;
   return fcntl( fd, F_SETFL, flags | O_NONBLOCK );
#else
   flags = 1;
      return ioctl( fd, FIOBIO, &flags );
#endif
}

const char* wrn_duffer_size = "not enough buffer size\n";
void back_trace( int _fd )
{
#if 1
   static const size_t buffer_size = 256;
   void* buffer[ buffer_size ];

   int nptrs = backtrace( buffer, buffer_size );
   if( buffer_size == nptrs )
   {
      SYS_SIMPLE_WRN( "%s", wrn_duffer_size );
      if( 0 < _fd )
         write( _fd, wrn_duffer_size, strlen( wrn_duffer_size ) );
   }
   SYS_SIMPLE_MSG( "backtrace() returned %d addresses:", nptrs );


   if( 0 < _fd )
   {
      SYS_SIMPLE_MSG( "dump backtrace to %d", _fd );
      backtrace_symbols_fd( buffer, nptrs, _fd );
   }
   else
   {
      char** strings = backtrace_symbols( buffer, nptrs );
      if( strings == nullptr )
      {
         SYS_SIMPLE_ERR( "backtrace_symbols" );
         return;
      }

      for( int i = 0; i < nptrs; ++i )
      {
         SYS_SIMPLE_MSG( "%s", strings[ i ] );
      }
      free( strings );
   }
#endif
}



} // namespace base::os::linux
