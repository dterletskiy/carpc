#pragma once

// Framework
#include "api/sys/application/RootComponent.hpp"
// Application
#include "imp/symbiont/events/AppEvent.hpp"
#include "imp/symbiont/components/onoff/server/OnOff.hpp"
#include "imp/symbiont/components/onoff/client/OnOff.hpp"
#include "imp/symbiont/components/onoff/client/SomeIP.hpp"



namespace symbiont::components::onoff {

   class Component
      : public base::application::RootComponent
      , public events::AppEvent::Consumer
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
         void process_event( const events::AppEvent::Event& ) override;

      private:
         interface::onoff::Server m_server_onoff;
         interface::onoff::Client m_client_onoff;

         interface::someip::Client m_client_someip;
   };

} // namespace symbiont::components::onoff
