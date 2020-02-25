// Application
#include "imp/app/events/PingEvent.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "PingEvent"



namespace application::events {

   PingEventData::PingEventData( )
   {
   }

   PingEventData::PingEventData( const std::string& _message )
      : message( _message )
   {
   }

   PingEventData::PingEventData( const PingEventData& data )
      : message( data.message )
   {
   }

   PingEventData::PingEventData( base::ByteBufferT& buffer )
   {
      from_buffer( buffer );
   }

   PingEventData::~PingEventData( )
   {
   }

   bool PingEventData::to_buffer( base::ByteBufferT& buffer ) const
   {
      return buffer.push( message );
   }

   bool PingEventData::from_buffer( base::ByteBufferT&  buffer )
   {
      return buffer.pop( message );
   }

} // namespace application::events


INIT_EVENT( application::events::NoID::PingEvent );
INIT_EVENT( application::events::ID::PingEvent );
INIT_EVENT( application::events::IPC::PingEvent );
