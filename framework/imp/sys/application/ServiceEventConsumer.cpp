#include "api/sys/dsi/Types.hpp"
#include "api/sys/application/Process.hpp"
#include "api/sys/application/SendReceive.hpp"
#include "imp/sys/application/ServiceEventConsumer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "ServiceEventConsumer"



namespace ev_i = base::events::service;
using namespace base::application;



ServiceEventConsumer::ServiceEventConsumer( SendReceive& send_receive )
   : m_send_receive( send_receive )
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
   const base::service::Passport& service_passport = *(event.data( ));
   SYS_INF( "id: %s / passport: %s", ev_i::c_str( id ), service_passport.name( ).c_str( ) );

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

   const dsi::SocketCongiguration configuration = static_cast< dsi::SocketCongiguration >( Process::instance( )->configuration( ).ipc_app.socket );
   dsi::Packet packet( command, service_passport, configuration );
   m_send_receive.send( packet, application::Context::invalid );
}
