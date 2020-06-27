#pragma once

// Framework
#include "api/sys/comm/event/Event.hpp"



namespace application::events {

   struct AppEventData
   {
      AppEventData( ) { }
      AppEventData( const std::string& _message ) : message( _message ) { }
      AppEventData( const AppEventData& data ) : message( data.message ) { }
      AppEventData( base::dsi::tByteStream& stream ) { from_stream( stream ); }
      ~AppEventData( ) { }

      bool to_stream( base::dsi::tByteStream& stream ) const { return stream.push( message ); }
      bool from_stream( base::dsi::tByteStream& stream ) { return stream.pop( message ); }

      std::string message = "";
   };

   enum class eAppEventID { BOOT, SHUTDOWN, PING, UNDEFINED };
   const char* c_str( const eAppEventID );

   using tAppEventSignature = base::TSignatureID< eAppEventID >;

   DEFINE_IPC_EVENT( AppEvent, AppEventData, tAppEventSignature );

} // namespace application::events
