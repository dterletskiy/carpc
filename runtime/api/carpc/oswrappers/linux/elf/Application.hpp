#include "carpc/oswrappers/linux/elf/File.hpp"



namespace carpc::os::os_linux {

   struct Application : public File
   {
      Application( int pid = 0 );

      struct Proc
      {
         char exe[ 1024 ];
      } proc;
   };

} // namespace carpc::os::os_linux
