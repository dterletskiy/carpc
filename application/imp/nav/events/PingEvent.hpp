#pragma once

// Framework
#include "api/sys/comm/Event.hpp"



namespace application::events {



enum class ePing : size_t
{
   ping,
   response
};

struct PingEventData
{
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
