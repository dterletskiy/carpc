#pragma once

// Framework
#include "api/sys/component/Component.hpp"
// Application
#include "imp/app/events/PingEvent.hpp"
#include "imp/app/clients/OnOff/Client.hpp"



namespace application::components::slave {



class Component
   : public base::Component
   , public events::ID::PingEvent::Consumer
{
public:
   static base::IComponent::tSptr creator( base::IServiceThread::tSptr );

private:
   Component( const base::IServiceThread::tSptr, const std::string& );
public:
   ~Component( ) override;

private:
   void process_event( const events::ID::PingEvent::Event& ) override;

private:
   clients::onoff::Client* mp_client_onoff = nullptr;
};



} // namespace application::components::slave
