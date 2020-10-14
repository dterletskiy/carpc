#include "api/sys/dsi/Types.hpp"
#include "api/sys/application/Process.hpp"
#include "api/sys/application/ThreadIPC.hpp"
#include "imp/sys/application/ServiceEventConsumer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "ServiceEventConsumer"



namespace ev_i = base::events::service;
using namespace base::application;



ServiceEventConsumer::ServiceEventConsumer( ThreadIPC& service )
   : m_service( service )
{
   ev_i::Action::Event::set_notification( this, { ev_i::eAction::RegisterServer } );
   ev_i::Action::Event::set_notification( this, { ev_i::eAction::UnregisterServer } );
   ev_i::Action::Event::set_notification( this, { ev_i::eAction::RegisterClient } );
   ev_i::Action::Event::set_notification( this, { ev_i::eAction::UnregisterClient } );
}

ServiceEventConsumer::~ServiceEventConsumer( )
{
   ev_i::Action::Event::clear_all_notifications( this );
}

void ServiceEventConsumer::process_event( const ev_i::Action::Event& event )
{
   const auto& id = event.info( ).id( );
   const base::service::Signature& signature = event.data( )->signature;
   const base::service::Address& address = event.data( )->address;
   SYS_INF( "id: %s / signature: %s / address: %s", ev_i::c_str( id ), signature.name( ).c_str( ), address.name( ).c_str( ) );

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

   const auto& configuration = Process::instance( )->configuration( );
   dsi::Packet packet;
   packet.add_package( command, signature, address, configuration.ipc_app.address, configuration.ipc_app.port );
   dsi::tByteStream stream;
   stream.push( packet );
   // m_service.send( stream );
}
