#pragma once

// Framework
#include "api/sys/comm/event/Event.hpp"



namespace application::events {

   struct AppEventData
   {
      AppEventData( ) { }
      AppEventData( const std::string& _message ) : message( _message ) { }
      AppEventData( const AppEventData& data ) : message( data.message ) { }
      AppEventData( base::ByteBufferT& buffer ) { from_buffer( buffer ); }
      ~AppEventData( ) { }

      bool to_buffer( base::ByteBufferT& buffer ) const { return buffer.push( message ); }
      bool from_buffer( base::ByteBufferT& buffer ) { return buffer.pop( message ); }

      std::string message = "";
   };

   enum class eAppEventID { BOOT, SHUTDOWN, PING, UNDEFINED };
   const char* c_str( const eAppEventID );

   using tAppEventSignature = base::TSignatureID< eAppEventID >;

   DEFINE_EVENT( AppEvent, AppEventData, tAppEventSignature );

} // namespace application::events
