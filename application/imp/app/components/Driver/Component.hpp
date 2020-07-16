#pragma once

// Framework
#include "api/sys/component/Component.hpp"
// Application
#include "imp/app/events/AppEvent.hpp"
#include "imp/app/clients/OnOff/Client.hpp"



namespace application::components::driver {



class Component
   : public base::Component
   , public events::AppEvent::Consumer
{
public:
   static base::IComponent::tSptr creator( base::IServiceThread& service );

private:
   Component( base::IServiceThread& service, const std::string& );
public:
   ~Component( ) override;

private:
   void process_event( const events::AppEvent::Event& ) override;

private:
   clients::onoff::Client* mp_client_onoff = nullptr;
};



} // namespace application::components::driver