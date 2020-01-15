#pragma once

#include "comm/Event.hpp"



namespace base {



enum class eServiceCommand : std::uint8_t
{
   boot,
   shutdown,
   ping,
};

class ServiceEventData
{
public:
   ServiceEventData( const eServiceCommand _command, const std::string& _info )
      : command( _command )
      , info( _info )
   { }
   ServiceEventData( const ServiceEventData& data )
   {
      command = data.command;
      info = data.info;
   }
   ~ServiceEventData( ) { };

   eServiceCommand   command;
   std::string       info;
};
DECLARE_EVENT( ServiceEvent, ServiceEventData, IServiceEventConsumer );



enum class eSysCommand : std::uint8_t
{
   boot,
   shutdown,
   ping,
};

class SysEventData
{
public:
   SysEventData( const eSysCommand _command, const std::string& _info )
      : command( _command )
      , info( _info )
   { }
   SysEventData( const SysEventData& data )
   {
      command = data.command;
      info = data.info;
   }
   ~SysEventData( ) { };

   eSysCommand    command;
   std::string    info;
};
DECLARE_EVENT( SysEvent, SysEventData, ISysEventConsumer );



}
