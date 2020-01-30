#pragma once

// Framework
#include "api/sys/component/Component.hpp"
// Application
#include "imp/nav/events/PingEvent.hpp"



namespace application::master {



base::ComponentPtr creator( base::ServiceThreadPtr );



class Master
   : public base::Component
   , public events::PingMasterEvent::Consumer
   , public events::PingSlaveEvent::Consumer
{
public:
   Master( const base::ServiceThreadPtr, const std::string& );
   ~Master( ) override;

private:
   void process_event( const events::PingMasterEvent::Event& ) override;
   void process_event( const events::PingSlaveEvent::Event& ) override;
};



} // namespace application::master
