#pragma once

// Framework
#include "api/sys/component/Component.hpp"
// Application
#include "imp/nav/events/PingEvent.hpp"



namespace application::driver {



base::ComponentPtr creator( base::ServicePtr );



class Driver
   : public base::Component
   , public events::IPingDriverEventConsumer
   , public events::IPingMasterEventConsumer
{
public:
   Driver( const base::ServicePtr, const std::string& );
   ~Driver( ) override;

private:
   void process_event( const events::PingDriverEvent& ) override;
   void process_event( const events::PingMasterEvent& ) override;
};



} // namespace application::driver
