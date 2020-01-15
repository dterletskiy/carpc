#pragma once

// Framework
#include "api/sys/comm/Event.hpp"



namespace api::onoff {



struct BaseData
{

};
using tBaseDataPtr = std::shared_ptr< BaseData >;

struct RequestTriggerStateData : public BaseData
{
   RequestTriggerStateData( const std::string& _state )
      : BaseData( )
      , state( _state )
   { }

   std::string state;
};

struct ResponseTriggerStateData : public BaseData
{
   ResponseTriggerStateData( const bool _result )
      : BaseData( )
      , result( _result )
   { }

   bool result;
};

struct NotificationCurrentStateData : public BaseData
{
   NotificationCurrentStateData( const std::string& _state )
      : BaseData( )
      , state( _state )
   { }

   std::string state;
};

DEFINE_ENUM_EX( eOnOff, size_t, RequestTriggerState, ResponseTriggerState, NotificationCurrentState );
struct OnOffEventData
{
   OnOffEventData( tBaseDataPtr _ptr ) : ptr( _ptr ) { }

   tBaseDataPtr ptr;
};



DECLARE_EVENT( OnOffEvent, api::onoff::OnOffEventData, api::onoff::eOnOff );



} // namespace api::onoff
