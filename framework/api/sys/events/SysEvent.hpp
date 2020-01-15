#pragma once

#include "api/sys/comm/Event.hpp"



namespace base {



enum class eSysCommand : size_t
{
   boot,
   shutdown,
   ping,
};

class SysEventData
{
   friend ByteBuffer& operator << ( ByteBuffer&, const SysEventData& );
   friend ByteBuffer& operator >> ( ByteBuffer&, SysEventData& );

public:
   SysEventData( );
   SysEventData( const eSysCommand, const std::string& );
   SysEventData( const SysEventData& );
   ~SysEventData( );

   eSysCommand    command;
   std::string    info;
};
DECLARE_EVENT( SysEvent, SysEventData, ISysEventConsumer );



} // namespace base
