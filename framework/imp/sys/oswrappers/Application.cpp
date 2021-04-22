#include "api/sys/oswrappers/Application.hpp"



using namespace base::os::os_linux;



Application::Application( int pid )
{
   if( 0 == pid )
      pid = getpid( );

   sprintf( proc.exe, "/proc/%d/exe", pid );

   if( -1 == readlink( proc.exe, path, sizeof( path ) ) )
   {
      error = errno;
      printf( "readlink(%s) error: %s\n", proc.exe, strerror( errno ) );
   }
}
