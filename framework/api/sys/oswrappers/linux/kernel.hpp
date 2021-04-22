#pragma once



namespace base::os::os_linux {

   int set_nonblock( int );
   void back_trace( int fd = 0 );

} // namespace base::os::os_linux
