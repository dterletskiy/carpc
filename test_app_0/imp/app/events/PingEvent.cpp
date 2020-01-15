#include "api/sys/common/Helpers.hpp"
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
      if( false == buffer.push( info ) )
         return false;

      return true;
   }

   bool PingEventData::from_buffer( base::ByteBufferT&  buffer )
   {
      if( false == buffer.pop( info ) )
         return false;

      return true;
   }

} // namespace application::events



INIT_DSI_EVENT( ServiceDSI, PingEvent );
