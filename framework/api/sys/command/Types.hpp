#pragma once

#include "api/sys/common/ID.hpp"
#include "api/sys/comm/async/event/Event.hpp"



namespace base::command {

   namespace queue {

      class Queue;
      using tID = base::TID< Queue >;

   }

   namespace command {

      class Command;
      using tID = base::TID< Command >;

   }

} // namespace base::events
