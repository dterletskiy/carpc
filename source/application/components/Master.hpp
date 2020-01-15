#pragma once

#include "component/Component.hpp"
#include "application/events/Events.hpp"



namespace application::master {



base::IComponentPtr creator( base::IServicePtr );



class Master
   : public base::Component
   , public events::IPingMasterEventConsumer
   , public events::IPingSlaveEventConsumer
{
public:
   Master( const base::IServicePtr, const std::string& );
   ~Master( ) override;

private:
   void process_event( const events::PingMasterEvent& ) override;
   void process_event( const events::PingSlaveEvent& ) override;
};



} // namespace application::master
