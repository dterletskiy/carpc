#include "Events.hpp"



namespace application::events {



   // PingEventData::PingEventData( const ePing _type, const std::string& _info )
   //    : type( _type )
   //    , info( _info )
   // {
   // }

   // PingEventData::PingEventData( const PingEventData& data )
   //    : type( data.type )
   //    , info( data.info )
   // {
   // }

   // PingEventData::PingEventData( base::ByteBuffer& buffer )
   // {
   //    buffer >> *this;
   // }

   // PingEventData::~PingEventData( )
   // {
   // }

   // base::ByteBuffer& operator << ( base::ByteBuffer& buffer, const PingEventData& data )
   // {
   //    buffer << static_cast< std::uint8_t >( data.type );
   //    buffer << data.info;
   //    return buffer;
   // }

   // base::ByteBuffer& operator >> ( base::ByteBuffer& buffer, PingEventData& data )
   // {
   //    buffer >> data.info;
   //    std::uint8_t _type;
   //    buffer >> _type;
   //    data.type = static_cast< ePing >( _type );
   //    return buffer;
   // }



   INIT_EVENT( PingDriverEvent );
   INIT_EVENT( PingMasterEvent );
   INIT_EVENT( PingSlaveEvent );

} // namespace application::events
