// Application
#include "imp/app/events/PingEvent.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "PingEvent"



namespace application::events {

   PingEventData::PingEventData( )
   {
   }

   PingEventData::PingEventData( const std::string& _info )
      : info( _info )
   {
   }

   PingEventData::PingEventData( const PingEventData& data )
      : info( data.info )
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
      return buffer.push( info );
   }

   bool PingEventData::from_buffer( base::ByteBufferT&  buffer )
   {
      return buffer.pop( info );
   }

} // namespace application::events


INIT_EVENT( application::events::NoID::PingEvent );
INIT_EVENT( application::events::ID::PingEvent );
INIT_EVENT( application::events::IPC::PingEvent );
