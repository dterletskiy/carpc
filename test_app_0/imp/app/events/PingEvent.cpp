// Application
#include "imp/app/events/PingEvent.hpp"



namespace application::events {

   PingEventData::PingEventData( )
   {
   }

   PingEventData::PingEventData( const ePing _type, const std::string& _info )
      : type( _type )
      , info( _info )
   {
   }

   PingEventData::PingEventData( const PingEventData& data )
      : type( data.type )
      , info( data.info )
   {
   }

   PingEventData::PingEventData( base::ByteBuffer& buffer )
   {
      buffer >> *this;
   }

   PingEventData::~PingEventData( )
   {
   }

   bool PingEventData::to_buffer( base::ByteBuffer& buffer ) const
   {
      if( false == buffer.push( static_cast< size_t >( type ) ) )
         return false;

      if( false == buffer.push( info ) )
         return false;

      return true;
   }

   bool PingEventData::from_buffer( base::ByteBuffer&  buffer )
   {
      if( false == buffer.pop( info ) )
         return false;

      size_t _type;
      if( false == buffer.pop( _type ) )
         return false;
      type = static_cast< ePing >( _type );

      return true;
   }

   base::ByteBuffer& operator << ( base::ByteBuffer& buffer, const PingEventData& data )
   {
      buffer << static_cast< size_t >( data.type );
      buffer << data.info;
      return buffer;
   }

   base::ByteBuffer& operator >> ( base::ByteBuffer& buffer, PingEventData& data )
   {
      buffer >> data.info;
      size_t _type;
      buffer >> _type;
      data.type = static_cast< ePing >( _type );
      return buffer;
   }

   INIT_EVENT( PingEvent );

} // namespace application::events



INIT_DSI_EVENT( DsiPingEvent, DsiService );
INIT_DSI_EVENT( xDsiPingEvent, xDsiService );

// REGISTER_DSI_EVENT( DsiPingEvent, DsiService );
// EventRegistry::instance( )->dump( );
// DsiService::DsiPingEvent::send_event( { eServiceCommand::boot, "fuck" } );
