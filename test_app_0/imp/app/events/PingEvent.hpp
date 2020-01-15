#pragma once

// Framework
#include "api/sys/comm/Event.hpp"



namespace application::events {



struct PingEventData
{
public:
   PingEventData( );
   PingEventData( const std::string& );
   PingEventData( const PingEventData& );
   PingEventData( base::ByteBufferT& );
   ~PingEventData( );

   bool to_buffer( base::ByteBufferT& ) const;
   bool from_buffer( base::ByteBufferT& );

   std::string    info = "";
};



enum class eEventID : size_t { request, response };



} // namespace application::events



DECLARE_DSI_EVENT( ServiceDSI, PingEvent, application::events::PingEventData );
