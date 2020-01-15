#pragma once

#include "component/Component.hpp"
#include "application/events/Events.hpp"



namespace application::driver {



base::IComponentPtr creator( base::IServicePtr );



class Driver
   : public base::Component
   , public events::IPingDriverEventConsumer
   , public events::IPingMasterEventConsumer
{
public:
   Driver( const base::IServicePtr, const std::string& );
   ~Driver( ) override;

private:
   void process_event( const events::PingDriverEvent& ) override;
   void process_event( const events::PingMasterEvent& ) override;
};



} // namespace application::driver
