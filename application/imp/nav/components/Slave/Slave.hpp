#pragma once

// Framework
#include "api/sys/component/Component.hpp"
// Application
#include "imp/nav/events/PingEvent.hpp"



namespace application::slave {



base::ComponentPtr creator( base::ServiceThreadPtr );



class Slave
   : public base::Component
   , public events::PingSlaveEvent::Consumer
{
public:
   Slave( const base::ServiceThreadPtr, const std::string& );
   ~Slave( ) override;

private:
   void process_event( const events::PingSlaveEvent::Event& ) override;
};



} // namespace application::slave
