#pragma once

#include <sys/time.h>
#include <linux/futex.h>



namespace carpc::os::os_linux {

   int set_nonblock( int );
   int futex( int* uaddr, int futex_op, int val, const struct timespec* timeout, int* uaddr2, int val3 );

} // namespace carpc::os::os_linux
