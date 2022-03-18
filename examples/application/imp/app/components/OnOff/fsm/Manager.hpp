#pragma once

#include "api/sys/fsm/TStateManager.hpp"
#include "imp/app/components/OnOff/fsm/Data.hpp"



namespace application::components::onoff {

   class Manager : public carpc::fsm::TStateManager< Types >
   {
      public:
         using tBase = carpc::fsm::TStateManager< Types >;

      public:
         Manager( );
         ~Manager( ) override = default;

      private:
         Subscriber::tSptr mp_subscriber = nullptr;
   };

} // namespace carpc::events
