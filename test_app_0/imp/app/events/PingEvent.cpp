// Application
#include "imp/app/events/PingEvent.hpp"



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
      // if( false == buffer.push( static_cast< size_t >( type ) ) )
      //    return false;

      if( false == buffer.push( info ) )
         return false;

      return true;
   }

   bool PingEventData::from_buffer( base::ByteBufferT&  buffer )
   {
      if( false == buffer.pop( info ) )
         return false;

      // size_t _type;
      // if( false == buffer.pop( _type ) )
      //    return false;
      // type = static_cast< ePing >( _type );

      return true;
   }

   INIT_EVENT( PingEventETC );
   INIT_EVENT( PingEventITC );



   INIT_EVENT( EventEx );

} // namespace application::events



INIT_DSI_EVENT_EX( ServiceDSI, PingEventDSI );
