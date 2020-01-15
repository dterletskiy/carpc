#include "api/sys/events/ServiceEvent.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "BaseEvents"



namespace base {

   ServiceEventData::ServiceEventData( )
   {
   }

   ServiceEventData::ServiceEventData( const eServiceCommand _command, const std::string& _info )
      : command( _command )
      , info( _info )
   {
   }

   ServiceEventData::ServiceEventData( const ServiceEventData& data )
      : command( data.command )
      , info( data.info )
   {
   }

   ServiceEventData::ServiceEventData( ByteBuffer& buffer )
   {
      buffer >> *this;
   }

   ServiceEventData::~ServiceEventData( )
   {
   }

   ByteBuffer& operator << ( ByteBuffer& buffer, const ServiceEventData& data )
   {
      buffer << static_cast< size_t >( data.command );
      buffer << data.info;
      return buffer;
   }

   ByteBuffer& operator >> ( ByteBuffer& buffer, ServiceEventData& data )
   {
      buffer >> data.info;
      size_t _command;
      buffer >> _command;
      data.command = static_cast< eServiceCommand >( _command );
      return buffer;
   }



   INIT_EVENT( ServiceEvent );

} // namespace base



INIT_DSI_EVENT( DsiServiceEvent, DsiService );
