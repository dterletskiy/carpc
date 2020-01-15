#pragma once

// Framework
#include "api/sys/component/RootComponent.hpp"
// Application
#include "imp/app/events/PingEvent.hpp"



namespace application::onoff {



base::ComponentPtr creator( base::ServicePtr );



class OnOff
   : public base::RootComponent
   , public events::PingEventETC::Consumer
   , public events::PingEventITC::Consumer
   , public ServiceDSI::PingEventDSI::Consumer
   , public events::EventEx::Consumer
{
public:
   OnOff( const base::ServicePtr, const std::string& );
   ~OnOff( ) override;

private:
   bool boot( const std::string& ) override;
   void process_event( const events::PingEventETC::Event& ) override;
   void process_event( const events::PingEventITC::Event& ) override;
   void process_event( const ServiceDSI::PingEventDSI::Event& ) override;
   void process_event( const events::EventEx::Event& ) override;
};



} // namespace application::onoff
