#pragma once

// Framework
#include "api/sys/component/Component.hpp"
// Application
#include "imp/nav/events/PingEvent.hpp"



namespace application::master {



base::ComponentPtr creator( base::ServicePtr );



class Master
   : public base::Component
   , public events::IPingMasterEventConsumer
   , public events::IPingSlaveEventConsumer
{
public:
   Master( const base::ServicePtr, const std::string& );
   ~Master( ) override;

private:
   void process_event( const events::PingMasterEvent& ) override;
   void process_event( const events::PingSlaveEvent& ) override;
};



} // namespace application::master
