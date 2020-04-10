#pragma once

// Framework
#include "api/sys/component/RootComponent.hpp"
// Application
#include "imp/app/events/PingEvent.hpp"
#include "imp/app/components/OnOff/server/Server.hpp"



namespace application::components::onoff {



class Component
   : public base::RootComponent
   , public events::NoID::PingEvent::Consumer
   , public events::ID::PingEvent::Consumer
   , public events::IPC::PingEvent::Consumer
{
public:
   static base::IComponent::tSptr creator( base::IServiceThread::tSptr );

private:
   Component( const base::IServiceThread::tSptr, const std::string& );
public:
   ~Component( ) override;

private:
   bool boot( const std::string& ) override;

private:
   void process_event( const events::NoID::PingEvent::Event& ) override;
   void process_event( const events::ID::PingEvent::Event& ) override;
   void process_event( const events::IPC::PingEvent::Event& ) override;

private:
   Server m_server_onoff;
};



} // namespace application::components::onoff
