#pragma once

// Framework
#include "api/sys/comm/Event.hpp"



namespace application::events {



enum class ePing : size_t
{
   ping,
   response
};

class PingEventData
{
   friend base::ByteBuffer& operator << ( base::ByteBuffer&, const PingEventData& );
   friend base::ByteBuffer& operator >> ( base::ByteBuffer&, PingEventData& );

public:
   PingEventData( );
   PingEventData( const ePing, const std::string& );
   PingEventData( const PingEventData& );
   PingEventData( base::ByteBuffer& );
   ~PingEventData( );

   bool to_buffer( base::ByteBuffer& ) const;
   bool from_buffer( base::ByteBuffer& );

   ePing          type;
   std::string    info;
};



DECLARE_EVENT( PingEvent, PingEventData, IPingEventConsumer );



} // namespace application::events



using DsiPingEventData = application::events::PingEventData;
DECLARE_DSI_EVENT( DsiPingEvent, DsiPingEventData, IDsiPingEventConsumer, DsiService );
using xDsiPingEventData = application::events::PingEventData;
DECLARE_DSI_EVENT( xDsiPingEvent, xDsiPingEventData, IxDsiPingEventConsumer, xDsiService );
