#pragma once

// Framework
#include "api/sys/component/RootComponent.hpp"
#include "api/sys/comm/timer/Timer.hpp"
// Application
#include "imp/core/components/onoff/server/Server.hpp"
#include "imp/core/components/onoff/client/Client.hpp"



namespace core::components::onoff {

   class Component
      : public base::RootComponent
   {
      public:
         static base::IComponent::tSptr creator( base::IServiceThread& service );

      private:
         Component( base::IServiceThread& service, const std::string& );
      public:
         ~Component( ) override;

      private:
         void boot( const std::string& ) override;

      private:
         Server m_server_onoff;
         Client m_client_onoff;

      public:
         void on_timer( const base::ID );
   };

} // namespace core::components::onoff
