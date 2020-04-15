#pragma once

#include "api/sys/comm/event/Event.hpp"



namespace base::events {

   namespace service {

      enum class eID : size_t { boot, shutdown, ping, };

      struct Data
      {
         std::string message;
      };
      DEFINE_EVENT( ServiceEvent, Data, TSignatureID< eID > );

   }

} // namespace base::events
