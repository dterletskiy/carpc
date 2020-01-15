#pragma once

#include "comm/Event.hpp"



namespace base {



enum class eServiceCommand : std::uint8_t
{
   boot,
   shutdown,
   ping,
};

class ServiceEventData
{
   friend ByteBuffer& operator << ( ByteBuffer&, const ServiceEventData& );
   friend ByteBuffer& operator >> ( ByteBuffer&, ServiceEventData& );

public:
   ServiceEventData( );
   ServiceEventData( const eServiceCommand, const std::string& );
   ServiceEventData( const ServiceEventData& );
   ServiceEventData( ByteBuffer& );
   ~ServiceEventData( );

   eServiceCommand   command;
   std::string       info;
};
DECLARE_EVENT( ServiceEvent, ServiceEventData, IServiceEventConsumer );



enum class eSysCommand : std::uint8_t
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



using DsiServiceEventData = base::ServiceEventData;
DECLARE_DSI_EVENT( DsiServiceEvent, base::ServiceEventData, IDsiServiceEventConsumer, DsiService );
