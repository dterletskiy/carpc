#pragma once

// Framework
#include "api/sys/application/RootComponent.hpp"
// Application
#include "imp/hybrid/events/AppEvent.hpp"
#include "imp/hybrid/components/onoff/server/OnOff.hpp"
#include "imp/hybrid/components/onoff/client/OnOff.hpp"



namespace hybrid::components::onoff {

   class Component
      : public carpc::application::RootComponent
      , public events::AppEvent::Consumer
   {
      public:
         static carpc::application::IComponent::tSptr creator( );

      private:
         Component( const std::string& );
      public:
         ~Component( ) override;

      private:
         void process_boot( const std::string& ) override;

      private:
         void process_event( const events::AppEvent::Event& ) override;

      private:
         interface::onoff::Server m_server_onoff;
         interface::onoff::Client m_client_onoff;
   };

} // namespace hybrid::components::onoff
