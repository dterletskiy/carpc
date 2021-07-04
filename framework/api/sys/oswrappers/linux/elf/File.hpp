#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <cstdlib>



namespace base::os::os_linux {

   struct File
   {
      File( const char* const file = nullptr, const int flags = -1 );
      ~File( );

      bool init( const char* const, const int flags );

      bool open( int flags );
      bool close( );
      ssize_t read( void* buffer, const std::size_t size, const bool match_size = false );
      off_t lseek( const off_t offset, int whence, const bool match_size = false );

      char path[ 1024 ];
      int handle;

      int error;
   };

} // namespace base::os::os_linux
