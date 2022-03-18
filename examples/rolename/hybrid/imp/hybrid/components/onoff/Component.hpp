#pragma once

// Framework
#include "api/sys/application/RootComponent.hpp"
// Application
#include "imp/hybrid/components/onoff/server/OnOff.hpp"
#include "imp/hybrid/components/onoff/client/OnOff.hpp"
#include "imp/hybrid/components/onoff/server/SomeIP.hpp"



namespace hybrid::components::onoff {

   class Component
      : public carpc::application::RootComponent
   {
      public:
         static carpc::application::IComponent::tSptr creator( );

      private:
         Component( const std::string& );
      public:
         ~Component( ) override;

      private:
         void boot( const std::string& ) override;

      private:
         interface::onoff::Server m_server_onoff;
         interface::onoff::Client m_client_onoff;

         interface::someip::Server m_server_someip;
   };

} // namespace hybrid::components::onoff
