#pragma once

// Framework
#include "comm/Event.hpp"



namespace application::events {



enum class ePing : std::uint8_t
{
   ping,
   response
};

struct PingEventData
{
//    friend base::ByteBuffer& operator << ( base::ByteBuffer&, const PingEventData& );
//    friend base::ByteBuffer& operator >> ( base::ByteBuffer&, PingEventData& );

// public:
//    PingEventData( const ePing, const std::string& );
//    PingEventData( const PingEventData& );
//    PingEventData( base::ByteBuffer& );
//    ~PingEventData( );

   ePing          type;
   std::string    info;
};
struct PingDriverEventData : public PingEventData { };
struct PingMasterEventData : public PingEventData { };
struct PingSlaveEventData : public PingEventData { };



DECLARE_EVENT( PingDriverEvent, PingDriverEventData, IPingDriverEventConsumer );
DECLARE_EVENT( PingMasterEvent, PingMasterEventData, IPingMasterEventConsumer );
DECLARE_EVENT( PingSlaveEvent, PingSlaveEventData, IPingSlaveEventConsumer );



} // namespace application::events
