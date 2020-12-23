#pragma once

#include "api/sys/fsm/TStateManager.hpp"
#include "imp/app/components/OnOff/fsm/Data.hpp"



namespace application::components::onoff {

   class Manager : public base::fsm::TStateManager< Types >
   {
      public:
         using tBase = base::fsm::TStateManager< Types >;

      public:
         Manager( );
         ~Manager( ) override = default;

      private:
         Subscriber::tSptr mp_subscriber = nullptr;
   };

} // namespace base::events
