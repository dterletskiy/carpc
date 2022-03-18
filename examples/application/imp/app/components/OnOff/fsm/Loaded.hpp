#pragma once

#include "api/sys/fsm/TState.hpp"
#include "imp/app/components/OnOff/fsm/Data.hpp"



namespace application::components::onoff {

   class Manager;



   class Loaded : public carpc::fsm::TState< Types >
   {
      public:
         Loaded( Manager& );
         ~Loaded( ) override = default;

      private:
         const std::optional< Types::tID > enter( ) override;
         void leave( ) override;

      private:
         const std::optional< Types::tID > process( const Data& ) override;

      private:
         Manager& m_manager;
   };

} // namespace carpc::events
