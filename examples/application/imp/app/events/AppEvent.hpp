#pragma once

// Framework
#include "api/sys/comm/async/event/Event.hpp"



namespace application::events {

   struct AppEventData
   {
      AppEventData( ) { }
      AppEventData( const std::string& _message ) : message( _message ) { }
      AppEventData( const AppEventData& data ) : message( data.message ) { }
      AppEventData( carpc::ipc::tStream& stream ) { from_stream( stream ); }
      ~AppEventData( ) { }

      bool to_stream( carpc::ipc::tStream& stream ) const { return carpc::ipc::serialize( stream, message ); }
      bool from_stream( carpc::ipc::tStream& stream ) { return carpc::ipc::deserialize( stream, message ); }

      std::string message = "";
   };

   enum class eAppEventID { BOOT, SHUTDOWN, PING, UNDEFINED };
   const char* c_str( const eAppEventID );

   using tAppEventSignature = carpc::async::id::TSignature< eAppEventID >;

   DEFINE_EVENT( AppEvent, AppEventData, tAppEventSignature );

} // namespace application::events
