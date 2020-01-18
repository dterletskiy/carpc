#pragma once

// Framework
#include "api/sys/comm/timer/Timer.hpp"
#include "api/sys/component/RootComponent.hpp"
// Application
#include "imp/app/events/PingEvent.hpp"
#include "imp/app/components/server/Server.hpp"
#include "imp/app/components/client/Client.hpp"



namespace application::onoff {



base::ComponentPtr creator( base::ServicePtr );



class OnOff
   : public base::RootComponent
   , public base::ITimerConsumer
   , public ServiceDSI::PingEvent::Consumer
{
public:
   OnOff( const base::ServicePtr, const std::string& );
   ~OnOff( ) override;

private:
   bool boot( const std::string& ) override;

private:
   void process_event( const ServiceDSI::PingEvent::Event& ) override;

private:
   Server m_server;
   Client m_client;

// Timer
private:
   void process_timer( const base::TimerID ) override;
   base::Timer m_timer;
};



} // namespace application::onoff
