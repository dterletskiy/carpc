#include "Events.hpp"

#include "Trace.hpp"
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
      buffer << static_cast< std::uint8_t >( data.command );
      buffer << data.info;
      return buffer;
   }

   ByteBuffer& operator >> ( ByteBuffer& buffer, ServiceEventData& data )
   {
      buffer >> data.info;
      std::uint8_t _command;
      buffer >> _command;
      data.command = static_cast< eServiceCommand >( _command );
      return buffer;
   }



   SysEventData::SysEventData( )
   {
   }

   SysEventData::SysEventData( const eSysCommand _command, const std::string& _info )
      : command( _command )
      , info( _info )
   {
   }

   SysEventData::SysEventData( const SysEventData& data )
      : command( data.command )
      , info( data.info )
   {
   }

   SysEventData::~SysEventData( )
   {
   }

   ByteBuffer& operator << ( ByteBuffer& buffer, const SysEventData& data )
   {
      buffer << static_cast< std::uint8_t >( data.command );
      buffer << data.info;
      return buffer;
   }

   ByteBuffer& operator >> ( ByteBuffer& buffer, SysEventData& data )
   {
      buffer >> data.info;
      std::uint8_t _command;
      buffer >> _command;
      data.command = static_cast< eSysCommand >( _command );
      return buffer;
   }


   INIT_EVENT( ServiceEvent );
   INIT_EVENT( SysEvent );

} // namespace base



INIT_DSI_EVENT( DsiServiceEvent, DsiService );
