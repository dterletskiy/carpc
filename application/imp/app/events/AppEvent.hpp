#pragma once

// Framework
#include "api/sys/comm/async/event/Event.hpp"



namespace application::events {

   struct AppEventData
   {
      AppEventData( ) { }
      AppEventData( const std::string& _message ) : message( _message ) { }
      AppEventData( const AppEventData& data ) : message( data.message ) { }
      AppEventData( base::ipc::tStream& stream ) { from_stream( stream ); }
      ~AppEventData( ) { }

      bool to_stream( base::ipc::tStream& stream ) const { return base::ipc::serialize( stream, message ); }
      bool from_stream( base::ipc::tStream& stream ) { return base::ipc::deserialize( stream, message ); }

      std::string message = "";
   };

   enum class eAppEventID { BOOT, SHUTDOWN, PING, UNDEFINED };
   const char* c_str( const eAppEventID );

   using tAppEventSignature = base::async::id::TSignature< eAppEventID >;

   DEFINE_EVENT( AppEvent, AppEventData, tAppEventSignature );

} // namespace application::events
