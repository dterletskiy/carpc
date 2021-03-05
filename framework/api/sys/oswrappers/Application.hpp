#include "api/sys/oswrappers/File.hpp"



namespace base::os::linux {

   struct Application : public File
   {
      Application( int pid = 0 );

      struct Proc
      {
         char exe[ 1024 ];
      } proc;
   };

} // namespace base::os::linux
