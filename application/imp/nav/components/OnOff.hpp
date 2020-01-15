#pragma once

// Framework
#include "api/sys/component/RootComponent.hpp"
// Application
#include "imp/nav/events/PingEvent.hpp"



namespace application::onoff {



base::ComponentPtr creator( base::ServicePtr );



class OnOff
   : public base::RootComponent
   , public events::IPingDriverEventConsumer
{
public:
   OnOff( const base::ServicePtr, const std::string& );
   ~OnOff( ) override;

private:
   bool boot( const std::string& ) override;
   void process_event( const events::PingDriverEvent& ) override;
};



} // namespace application::onoff
