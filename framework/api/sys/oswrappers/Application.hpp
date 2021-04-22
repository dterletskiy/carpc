#include "api/sys/oswrappers/File.hpp"



namespace base::os::os_linux {

   struct Application : public File
   {
      Application( int pid = 0 );

      struct Proc
      {
         char exe[ 1024 ];
      } proc;
   };

} // namespace base::os::os_linux
