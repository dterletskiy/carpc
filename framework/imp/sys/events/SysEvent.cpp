#include "api/sys/events/SysEvent.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "BaseEvents"



namespace base {

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
      buffer << static_cast< size_t >( data.command );
      buffer << data.info;
      return buffer;
   }

   ByteBuffer& operator >> ( ByteBuffer& buffer, SysEventData& data )
   {
      buffer >> data.info;
      size_t _command;
      buffer >> _command;
      data.command = static_cast< eSysCommand >( _command );
      return buffer;
   }


   INIT_EVENT( SysEvent );

} // namespace base
