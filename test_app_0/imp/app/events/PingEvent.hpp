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

   std::string    info;
};



DECLARE_EVENT( PingEventETC, PingEventData );
DECLARE_EVENT( PingEventITC, PingEventData );



enum class eEventID : size_t { request, response };
DECLARE_EVENT_EX( EventEx, PingEventData, eEventID );



} // namespace application::events



DECLARE_DSI_EVENT_EX( ServiceDSI, PingEventDSI, application::events::PingEventData, application::events::eEventID );
