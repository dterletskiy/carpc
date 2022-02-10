#pragma once

// Framework
#include "api/sys/application/Component.hpp"
// Application
#include "imp/app/events/AppEvent.hpp"
#include "imp/app/clients/OnOff/Client.hpp"



namespace application::components::driver {

   class Component
      : public base::application::Component
      , public events::AppEvent::Consumer
   {
      public:
         static base::application::IComponent::tSptr creator( );

      private:
         Component( const std::string& );
      public:
         ~Component( ) override;

      private:
         void process_event( const events::AppEvent::Event& ) override;

      private:
         clients::onoff::Client::tSptr mp_client_onoff = nullptr;
   };

} // namespace application::components::driver
