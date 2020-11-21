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

   const auto& configuration = Process::instance( )->configuration( );
   dsi::Packet packet( command, service_passport, configuration.ipc_app );
   m_service.send( packet, application::Context::invalid( ) );
}
