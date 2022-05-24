#pragma once

#include "carpc/common/ID.hpp"



namespace carpc::fsm {

   enum class eSignal : std::uint8_t { SYSTEM, STATE };
   const char* c_str( const eSignal& );

   namespace manager
   {
      class ManagerID;
      using ID = carpc::TID< ManagerID >;
   }

   namespace state
   {
      class StateID;
      using ID = carpc::TID< StateID >;
   }

} // namespace carpc::events
