#pragma once

#include "api/sys/common/ID.hpp"
#include "api/sys/common/Name.hpp"



namespace base::command {

   namespace queue
   {
      class QueueID;
      using ID = base::TID< QueueID >;
      using Name = base::TName< QueueID >;
   }

   namespace command
   {
      class CommandID;
      using ID = base::TID< CommandID >;
      using Name = base::TName< CommandID >;
   }

} // namespace base::events
