#pragma once

#include "api/sys/comm/Event.hpp"



namespace base {



enum class eServiceCommand : size_t
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



} // namespace base



using DsiServiceEventData = base::ServiceEventData;
DECLARE_DSI_EVENT( DsiServiceEvent, base::ServiceEventData, IDsiServiceEventConsumer, DsiService );
