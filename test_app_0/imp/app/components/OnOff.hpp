#pragma once

// Framework
#include "api/sys/component/RootComponent.hpp"
// Application
#include "imp/app/events/PingEvent.hpp"



namespace application::onoff {



base::ComponentPtr creator( base::ServicePtr );



class OnOff
   : public base::RootComponent
   , public events::IPingEventConsumer
   , public DsiService::IxDsiPingEventConsumer
{
public:
   OnOff( const base::ServicePtr, const std::string& );
   ~OnOff( ) override;

private:
   bool boot( const std::string& ) override;
   void process_event( const events::PingEvent& ) override;
   void process_event( const DsiService::xDsiPingEvent& ) override;
};



} // namespace application::onoff
