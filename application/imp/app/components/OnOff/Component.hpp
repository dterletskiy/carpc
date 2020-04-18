#pragma once

// Framework
#include "api/sys/component/RootComponent.hpp"
// Application
#include "imp/app/components/OnOff/server/Server.hpp"



namespace application::components::onoff {



class Component
   : public base::RootComponent
{
public:
   static base::IComponent::tSptr creator( base::IServiceThread::tSptr );

private:
   Component( const base::IServiceThread::tSptr, const std::string& );
public:
   ~Component( ) override;

private:
   void boot( const std::string& ) override;

private:
   Server m_server_onoff;
};



} // namespace application::components::onoff
