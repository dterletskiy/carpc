#pragma once

// Framework
#include "api/sys/comm/Event.hpp"



namespace api::onoff {



DEFINE_ENUM( eOnOff, size_t, RequestTriggerState, ResponseTriggerState, NotificationCurrentState, Undefined );

class BaseData;
using tBaseDataPtr = std::shared_ptr< BaseData >;



class BaseData
{
public:
   BaseData( const eOnOff );
   static tBaseDataPtr create( base::ByteBufferT& );

public:
   virtual bool to_buffer( base::ByteBufferT& ) = 0;
   virtual bool from_buffer( base::ByteBufferT& ) = 0;

public:
   virtual const eOnOff id( ) const final;
private:
   eOnOff m_id = eOnOff::Undefined;
};



class RequestTriggerStateData : public BaseData
{
public:
   RequestTriggerStateData( );
   RequestTriggerStateData( const std::string& );

public:
   bool to_buffer( base::ByteBufferT& ) override;
   bool from_buffer( base::ByteBufferT& ) override;

public:
   std::string state = "";
};



class ResponseTriggerStateData : public BaseData
{
public:
   ResponseTriggerStateData( );
   ResponseTriggerStateData( const bool );

public:
   bool to_buffer( base::ByteBufferT& ) override;
   bool from_buffer( base::ByteBufferT& ) override;

public:
   bool result = false;
};



class NotificationCurrentStateData : public BaseData
{
public:
   NotificationCurrentStateData( );
   NotificationCurrentStateData( const std::string& );

public:
   bool to_buffer( base::ByteBufferT& ) override;
   bool from_buffer( base::ByteBufferT& ) override;

public:
   std::string state = "";
};



class OnOffEventData
{
public:
   OnOffEventData( );
   OnOffEventData( tBaseDataPtr );

public:
   bool to_buffer( base::ByteBufferT& ) const;
   bool from_buffer( base::ByteBufferT& );

public:
   tBaseDataPtr ptr = nullptr;
};



} // namespace api::onoff



DECLARE_DSI_EVENT( ServiceOnOff, OnOffEvent, api::onoff::OnOffEventData, api::onoff::eOnOff );
