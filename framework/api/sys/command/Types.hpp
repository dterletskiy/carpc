#pragma once

#include "api/sys/common/ID.hpp"
#include "api/sys/common/Name.hpp"



namespace carpc::command {

   namespace queue
   {
      class QueueID;
      using ID = carpc::TID< QueueID >;
      using Name = carpc::TName< QueueID >;
   }

   namespace command
   {
      class CommandID;
      using ID = carpc::TID< CommandID >;
      using Name = carpc::TName< CommandID >;
   }

} // namespace carpc::events
