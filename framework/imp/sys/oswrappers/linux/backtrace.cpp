#include <string.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <execinfo.h>

#include "api/sys/oswrappers/linux/backtrace.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Backtrace"



namespace base::os::os_linux {

   const char* wrn_duffer_size = "not enough buffer size\n";
   void back_trace( int _fd )
   {
      static const size_t buffer_size = 256;
      void* buffer[ buffer_size ];

      int nptrs = backtrace( buffer, buffer_size );
      if( buffer_size == nptrs )
      {
         // SYS_WRN( "%s", wrn_duffer_size );
         if( 0 < _fd )
            write( _fd, wrn_duffer_size, strlen( wrn_duffer_size ) );
      }
      // SYS_VRB( "backtrace() returned %d addresses:", nptrs );


      if( 0 < _fd )
      {
         // SYS_VRB( "dump backtrace to %d", _fd );
         backtrace_symbols_fd( buffer, nptrs, _fd );
      }
      else
      {
         char** strings = backtrace_symbols( buffer, nptrs );
         if( strings == nullptr )
         {
            // SYS_ERR( "backtrace_symbols" );
            return;
         }

         for( int i = 0; i < nptrs; ++i )
         {
            // SYS_VRB( "%s", strings[ i ] );
         }
         free( strings );
      }
   }

} // namespace base::os::os_linux
