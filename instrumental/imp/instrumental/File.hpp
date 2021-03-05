#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>



namespace instrumental {

   struct File
   {
      File( const char* const file = nullptr, const int flags = -1 ) __attribute__(( no_instrument_function ));
      ~File( ) __attribute__(( no_instrument_function ));

      bool init( const char* const, const int flags ) __attribute__(( no_instrument_function ));

      bool open( int flags ) __attribute__(( no_instrument_function ));
      bool close( ) __attribute__(( no_instrument_function ));
      ssize_t read( void* buffer, const std::size_t size, const bool match_size = false ) __attribute__(( no_instrument_function ));
      off_t lseek( const off_t offset, int whence, const bool match_size = false ) __attribute__(( no_instrument_function ));

      char path[ 1024 ];
      int handle;

      int error;
   };

}
