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
public:
   PingEventData( );
   PingEventData( const ePing, const std::string& );
   PingEventData( const PingEventData& );
   PingEventData( base::ByteBufferT& );
   ~PingEventData( );

   bool to_buffer( base::ByteBufferT& ) const;
   bool from_buffer( base::ByteBufferT& );

   ePing          type;
   std::string    info;
};



DECLARE_EVENT( PingEventETC, PingEventData );
DECLARE_EVENT( PingEventITC, PingEventData );



} // namespace application::events



DECLARE_DSI_EVENT( ServiceDSI, PingEventDSI, application::events::PingEventData );
