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
   static base::ComponentPtr creator( base::ServiceThreadPtr );

private:
   Component( const base::ServiceThreadPtr, const std::string& );
public:
   ~Component( ) override;

private:
   void process_event( const events::ID::PingEvent::Event& ) override;

private:
   clients::onoff::Client* mp_client_onoff = nullptr;
};



} // namespace application::components::slave
