#pragma once

// Framework
#include "api/sys/comm/event/Event.hpp"



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

      std::string message = "";
   };



   enum class eEventID : size_t { boot, ping };



   namespace NoID {
      DECLARE_EVENT( PingEvent, PingEventData );
   }
   namespace ID {
      DECLARE_EVENT( PingEvent, PingEventData, eEventID );
   }
   namespace IPC {
      DECLARE_IPC_EVENT( PingEvent, PingEventData, eEventID );
   }



} // namespace application::events
