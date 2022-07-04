#include "carpc/common/Packet.hpp"
#include "carpc/application/Process.hpp"
#include "SendReceive.hpp"
#include "ServiceEventConsumer.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "ServiceEventConsumer"



namespace ev_i = carpc::events::service;
using namespace carpc::application;



ServiceEventConsumer::ServiceEventConsumer( SendReceive* const p_send_receive )
   : mp_send_receive( p_send_receive )
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
   const carpc::service::Passport& service_passport = *(event.data( ));
   SYS_INF( "id: %s / passport: %s", ev_i::c_str( id ), service_passport.dbg_name( ).c_str( ) );

   ipc::eCommand command = ipc::eCommand::Undefined;
   switch( id )
   {
      case ev_i::eAction::RegisterServer:
      {
         command = ipc::eCommand::RegisterServer;
         break;
      }
      case ev_i::eAction::UnregisterServer:
      {
         command = ipc::eCommand::UnregisterServer;
         break;
      }
      case ev_i::eAction::RegisterClient:
      {
         command = ipc::eCommand::RegisterClient;
         break;
      }
      case ev_i::eAction::UnregisterClient:
      {
         command = ipc::eCommand::UnregisterClient;
         break;
      }
      default: break;
   }

   if( ipc::eCommand::Undefined == command )
      return;

   const ipc::SocketCongiguration configuration = static_cast< ipc::SocketCongiguration >( Process::instance( )->configuration( ).ipc_app.socket );
   ipc::Packet packet( command, service_passport, configuration );
   const bool result = mp_send_receive->send( packet, application::Context::invalid );
   SYS_INF( "result = %d", result );
}
