#pragma once

#include "api/sys/common/ID.hpp"



namespace base::fsm {

   enum class eSignal : std::uint8_t { SYSTEM, STATE };
   const char* c_str( const eSignal& );

   namespace manager {

      class Manager;
      using tID = base::TID< Manager >;

   }

   namespace state {

      class State;
      using tID = base::TID< State >;

   }

} // namespace base::events
