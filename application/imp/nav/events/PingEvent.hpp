#pragma once

// Framework
#include "api/sys/comm/event/Event.hpp"



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



DECLARE_EVENT( PingDriverEvent, PingEventData );
DECLARE_EVENT( PingMasterEvent, PingEventData );
DECLARE_EVENT( PingSlaveEvent, PingEventData );



} // namespace application::events
