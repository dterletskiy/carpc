#pragma once

// Framework
#include "api/sys/component/Component.hpp"
// Application
#include "imp/app/events/AppEvent.hpp"
#include "imp/app/clients/OnOff/Client.hpp"



namespace application::components::master {



class Component
   : public base::Component
   , public events::AppEvent::Consumer
{
public:
   static base::IComponent::tSptr creator( base::IServiceThread::tSptr );

private:
   Component( const base::IServiceThread::tSptr, const std::string& );
public:
   ~Component( ) override;

private:
   void process_event( const events::AppEvent::Event& ) override;

private:
   clients::onoff::Client* mp_client_onoff = nullptr;
};



} // namespace application::components::master
