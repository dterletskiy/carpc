#pragma once

#include "component/Component.hpp"
#include "component/Root.hpp"
#include "application/events/Events.hpp"



namespace application::onoff {



base::IComponentPtr creator( base::IServicePtr );



class OnOff
   : public base::Root
   , public base::Component
   , public events::IPingDriverEventConsumer
{
public:
   OnOff( const base::IServicePtr, const std::string& );
   ~OnOff( ) override;

private:
   bool boot( const std::string& ) override;
   void process_event( const events::PingDriverEvent& ) override;
};



} // namespace application::onoff
