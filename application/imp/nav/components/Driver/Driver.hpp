#pragma once

// Framework
#include "api/sys/component/Component.hpp"
// Application
#include "imp/nav/events/PingEvent.hpp"



namespace application::driver {



base::ComponentPtr creator( base::ServicePtr );



class Driver
   : public base::Component
   , public events::PingDriverEvent::Consumer
   , public events::PingMasterEvent::Consumer
{
public:
   Driver( const base::ServicePtr, const std::string& );
   ~Driver( ) override;

private:
   void process_event( const events::PingDriverEvent::Event& ) override;
   void process_event( const events::PingMasterEvent::Event& ) override;
};



} // namespace application::driver
