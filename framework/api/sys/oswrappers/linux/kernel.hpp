#pragma once

#include <sys/time.h>
#include <linux/futex.h>



namespace base::os::os_linux {

   int set_nonblock( int );
   void back_trace( int fd = 0 );
   int futex( int* uaddr, int futex_op, int val, const struct timespec* timeout, int* uaddr2, int val3 );

} // namespace base::os::os_linux
