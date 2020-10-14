#pragma once

// Framework
#include "api/sys/component/Component.hpp"
// Application
#include "imp/app/events/AppEvent.hpp"
#include "imp/app/clients/OnOff/Client.hpp"



namespace application::components::slave {

   class Component
      : public base::component::Component
      , public events::AppEvent::Consumer
   {
      public:
         static base::component::IComponent::tSptr creator( base::application::IThread& service );

      private:
         Component( base::application::IThread& service, const std::string& );
      public:
         ~Component( ) override;

      private:
         void process_event( const events::AppEvent::Event& ) override;

      private:
         clients::onoff::Client::tSptr mp_client_onoff = nullptr;
   };

} // namespace application::components::slave
