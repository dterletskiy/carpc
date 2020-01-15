#include <fcntl.h>

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

} // namespace base::os::linux
