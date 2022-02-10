#pragma once

// Framework
#include "api/sys/application/RootComponent.hpp"
// Application
#include "imp/hybrid/components/onoff/server/Server.hpp"
#include "imp/hybrid/components/onoff/client/Client.hpp"



namespace hybrid::components::onoff {

   class Component
      : public base::application::RootComponent
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
         Client m_client_onoff;
   };

} // namespace hybrid::components::onoff
