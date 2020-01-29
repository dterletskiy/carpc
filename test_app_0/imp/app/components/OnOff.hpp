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
   , public events::NoID::PingEvent::Consumer
   , public events::ID::PingEvent::Consumer
   , public events::IPC::PingEvent::Consumer
{
public:
   OnOff( const base::ServicePtr, const std::string& );
   ~OnOff( ) override;

private:
   bool boot( const std::string& ) override;

private:
   void process_event( const events::NoID::PingEvent::Event& ) override;
   void process_event( const events::ID::PingEvent::Event& ) override;
   void process_event( const events::IPC::PingEvent::Event& ) override;

private:
   Server m_server_onoff_xxx;
   Client m_client_onoff_xxx;
private:
   Server m_server_onoff_yyy;
   Client m_client_onoff_yyy;

// Timer
private:
   void process_timer( const base::TimerID ) override;
   base::Timer m_timer;
};



} // namespace application::onoff
