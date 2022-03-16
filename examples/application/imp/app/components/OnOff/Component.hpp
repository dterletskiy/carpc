#pragma once

// Framework
#include "api/sys/application/RootComponent.hpp"
#include "api/sys/comm/timer/Timer.hpp"
#include "api/sys/fsm/TStateManager.hpp"
// Application
#include "imp/app/components/OnOff/fsm/Manager.hpp"
#include "imp/app/components/OnOff/server/Server.hpp"



namespace application::components::onoff {

   class Component
      : public base::application::RootComponent
      , public base::timer::ITimerConsumer
   {
      public:
         static base::application::IComponent::tSptr creator( );

      private:
         Component( const std::string& );
      public:
         ~Component( ) override;

      private:
         void boot( const std::string& ) override;

      private:
         Server m_server_onoff;

      // Timer
      private:
         void process_timer( const base::comm::timer::ID ) override;
         base::timer::Timer m_timer;

      public:
         void on_timer( const base::comm::timer::ID );

      private:
         Manager m_fsm;
   };

} // namespace application::components::onoff
