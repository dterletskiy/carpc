#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/file.h>



namespace instrumental {

   struct Application
   {
      Application( int pid = 0 ) __attribute__(( no_instrument_function ));

      void print( ) const __attribute__(( no_instrument_function ));

      bool open( int flags ) __attribute__(( no_instrument_function ));
      bool close( ) __attribute__(( no_instrument_function ));
      ssize_t read( void* buffer, const std::size_t size, const bool match_size = false ) __attribute__(( no_instrument_function ));
      off_t lseek( const off_t offset, int whence, const bool match_size = false ) __attribute__(( no_instrument_function ));

      struct
      {
         char exe[ 1024 ];
      } proc;

      char path[ 1024 ];
      int handle;

      int error;
   };

}
