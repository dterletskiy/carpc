#pragma once

#include "api/sys/comm/event/Event.hpp"



namespace base {



enum class eServiceCommand : size_t
{
   boot,
   shutdown,
   ping,
};

struct ServiceEventData
{
   eServiceCommand   command;
   std::string       info;
};
DECLARE_EVENT( ServiceEvent, ServiceEventData );



} // namespace base
