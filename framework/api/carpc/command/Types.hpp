#pragma once

#include "carpc/common/ID.hpp"
#include "carpc/common/Name.hpp"



namespace carpc::command {

   namespace queue
   {
      class QueueID;
      using ID = carpc::TID< QueueID >;
      using Name = carpc::TName< QueueID >;

      enum class eState : std::uint8_t
      {
         IDLE,
         RUNNING,
         PAUSING,
         PAUSED,
         RESUMING,
         STOPPING,
         STOPED
      };
      const char* c_str( const eState& );
   }

   namespace command
   {
      class CommandID;
      using ID = carpc::TID< CommandID >;
      using Name = carpc::TName< CommandID >;

      enum class eState : std::uint8_t
      {
         IDLE,
         EXECUTING,
         PAUSING,
         PAUSED,
         RESUMING,
         STOPPING,
         STOPED,
         FINISHED
      };
      const char* c_str( const eState& );
   }

} // namespace carpc::events
