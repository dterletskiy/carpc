#pragma once

// Framework
#include "api/sys/component/RootComponent.hpp"
// Application
#include "imp/nav/events/PingEvent.hpp"



namespace application::onoff {



base::ComponentPtr creator( base::ServiceThreadPtr );



class OnOff
   : public base::RootComponent
   , public events::PingDriverEvent::Consumer
{
public:
   OnOff( const base::ServiceThreadPtr, const std::string& );
   ~OnOff( ) override;

private:
   bool boot( const std::string& ) override;
   void process_event( const events::PingDriverEvent::Event& ) override;
};



} // namespace application::onoff
