#pragma once

// Framework
#include "api/sys/comm/event/Event.hpp"
#include "api/sys/helpers/macros/strings.hpp"



namespace api::onoff::no_dsi {



DEFINE_ENUM( eOnOff, size_t, RequestTriggerState, ResponseTriggerState, NotificationCurrentState, Undefined );



struct BaseData
{

};
using tBaseDataPtr = std::shared_ptr< BaseData >;



struct RequestTriggerStateData : public BaseData
{
   RequestTriggerStateData( const std::string&, const size_t );

   std::string state = "";
   size_t delay = 0;
};



struct ResponseTriggerStateData : public BaseData
{
   ResponseTriggerStateData( const bool );

   bool result = false;
};



struct NotificationCurrentStateData : public BaseData
{
   NotificationCurrentStateData( const std::string& );

   std::string state = "";
};



struct OnOffEventData
{
   OnOffEventData( tBaseDataPtr );

   tBaseDataPtr ptr = nullptr;
};



DECLARE_EVENT_RR( OnOffEvent, OnOffEventData, eOnOff );



} // namespace api::onoff::no_dsi
