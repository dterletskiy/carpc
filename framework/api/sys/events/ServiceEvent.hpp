#pragma once

#include "api/sys/comm/Event.hpp"



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
DECLARE_EVENT( ServiceEvent, ServiceEventData, IServiceEventConsumer );



} // namespace base
