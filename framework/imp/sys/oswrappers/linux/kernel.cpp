#include <string.h>
#include <fcntl.h>
#include <sys/syscall.h>

#include "api/sys/oswrappers/linux/kernel.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Kernel"



namespace base::os::os_linux {

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

   int futex( int* uaddr, int futex_op, int val, const struct timespec* timeout, int* uaddr2, int val3 )
   {
      return syscall( SYS_futex, uaddr, futex_op, val, timeout, uaddr, val3 );
   }

} // namespace base::os::os_linux
