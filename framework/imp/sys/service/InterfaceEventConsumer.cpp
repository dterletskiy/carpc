#include "api/sys/dsi/Types.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/service/ServiceIpcThread.hpp"
#include "imp/sys/service/InterfaceEventConsumer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "InterfaceEventConsumer"


namespace ev_i = base::events::interface;
using namespace base;



InterfaceEventConsumer::InterfaceEventConsumer( ServiceIpcThread& service )
   : m_service( service )
{
   ev_i::Action::Event::set_notification( this, { ev_i::eAction::RegisterServer } );
   ev_i::Action::Event::set_notification( this, { ev_i::eAction::UnregisterServer } );
   ev_i::Action::Event::set_notification( this, { ev_i::eAction::RegisterClient } );
   ev_i::Action::Event::set_notification( this, { ev_i::eAction::UnregisterClient } );
}

InterfaceEventConsumer::~InterfaceEventConsumer( )
{
   ev_i::Action::Event::clear_all_notifications( this );
}

void InterfaceEventConsumer::process_event( const ev_i::Action::Event& event )
{
   const auto& id = event.info( ).id( );
   const base::interface::Signature& signature = event.data( )->signature;
   const void* ptr = event.data( )->ptr;
   SYS_INF( "id: %s / signature: %s / ptr: %p", ev_i::c_str( id ), signature.name( ).c_str( ), ptr );

   dsi::eCommand command = dsi::eCommand::Undefined;
   switch( id )
   {
      case ev_i::eAction::RegisterServer:
      {
         command = dsi::eCommand::RegisterServer;
         break;
      }
      case ev_i::eAction::UnregisterServer:
      {
         command = dsi::eCommand::UnregisterServer;
         break;
      }
      case ev_i::eAction::RegisterClient:
      {
         command = dsi::eCommand::RegisterClient;
         break;
      }
      case ev_i::eAction::UnregisterClient:
      {
         command = dsi::eCommand::UnregisterClient;
         break;
      }
      default: break;
   }

   if( dsi::eCommand::Undefined == command )
      return;

   const auto& configuration = ServiceProcess::instance( )->configuration( );
   dsi::Packet packet;
   packet.add_package( command, signature, ptr, configuration.ipc_app.address, configuration.ipc_app.port );
   dsi::tByteStream stream;
   stream.push( packet );
   m_service.send( stream );
}
