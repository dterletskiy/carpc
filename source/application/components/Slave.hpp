#pragma once

#include "component/Component.hpp"
#include "application/events/Events.hpp"



namespace application::slave {



base::IComponentPtr creator( base::IServicePtr );



class Slave
   : public base::Component
   , public events::IPingSlaveEventConsumer
{
public:
   Slave( const base::IServicePtr, const std::string& );
   ~Slave( ) override;

private:
   void process_event( const events::PingSlaveEvent& ) override;
};



} // namespace application::slave
