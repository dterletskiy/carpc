#pragma once

// Framework
#include "api/sys/comm/async/event/Event.hpp"



namespace hybrid::events {

   struct AppEventData
   {
      AppEventData( ) = default;
      AppEventData( const std::string& _message ) : message( _message ) { }
      AppEventData( const AppEventData& data ) : message( data.message ) { }
      ~AppEventData( ) = default;

      std::string message = "";
   };

   enum class eAppEventID { BOOT, SHUTDOWN, PING, UNDEFINED };
   const char* c_str( const eAppEventID );

   using tAppEventSignature = carpc::async::id::TSignature< eAppEventID >;

   DEFINE_EVENT( AppEvent, AppEventData, tAppEventSignature );

} // namespace hybrid::events
