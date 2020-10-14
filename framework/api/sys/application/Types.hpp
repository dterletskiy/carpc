#pragma once

#include "api/sys/common/ID.hpp"



namespace base::application {

   class Process;
   namespace process {
      using ID = base::TID< Process >;
   }

   class Thread;
   namespace thread {
      using ID = base::TID< Thread >;
   }

} // namespace base::application
