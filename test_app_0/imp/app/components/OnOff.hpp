#pragma once

// Framework
#include "api/sys/component/RootComponent.hpp"
// Application
#include "imp/app/events/PingEvent.hpp"



namespace application::onoff {



base::ComponentPtr creator( base::ServicePtr );



class OnOff
   : public base::RootComponent
   , public ServiceDSI::PingEvent::Consumer
{
public:
   OnOff( const base::ServicePtr, const std::string& );
   ~OnOff( ) override;

private:
   bool boot( const std::string& ) override;
   void process_event( const ServiceDSI::PingEvent::Event& ) override;
};



} // namespace application::onoff
