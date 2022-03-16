#pragma once

#include "api/sys/common/ID.hpp"



namespace base::fsm {

   enum class eSignal : std::uint8_t { SYSTEM, STATE };
   const char* c_str( const eSignal& );

   namespace manager
   {
      class ManagerID;
      using ID = base::TID< ManagerID >;
   }

   namespace state
   {
      class StateID;
      using ID = base::TID< StateID >;
   }

} // namespace base::events
