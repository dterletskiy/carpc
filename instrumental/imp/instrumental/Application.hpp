#include "File.hpp"



namespace instrumental {

   struct Application : public File
   {
      Application( int pid = 0 ) __attribute__(( no_instrument_function ));

      struct Proc
      {
         char exe[ 1024 ];
      } proc;
   };

}
