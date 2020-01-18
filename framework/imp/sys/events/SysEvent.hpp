#pragma once

#include "api/sys/comm/event/Event.hpp"



namespace base {



enum class eSysCommand : size_t
{
   boot,
   shutdown,
   ping,
};

struct SysEventData
{
   eSysCommand    command;
   std::string    info;
};
DECLARE_EVENT( SysEvent, SysEventData );



} // namespace base
