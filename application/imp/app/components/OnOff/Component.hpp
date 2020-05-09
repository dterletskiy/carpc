#pragma once

// Framework
#include "api/sys/component/RootComponent.hpp"
#include "api/sys/comm/timer/Timer.hpp"
// Application
#include "imp/app/components/OnOff/server/Server.hpp"



namespace application::components::onoff {



class Component
   : public base::RootComponent
   , public base::ITimerConsumer
{
public:
   static base::IComponent::tSptr creator( base::IServiceThread& service );

private:
   Component( base::IServiceThread& service, const std::string& );
public:
   ~Component( ) override;

private:
   void boot( const std::string& ) override;

private:
   Server m_server_onoff;

// Timer
private:
   void process_timer( const base::TimerID ) override;
   base::Timer m_timer;
};



} // namespace application::components::onoff
