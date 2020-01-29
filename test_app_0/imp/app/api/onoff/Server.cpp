// Application
#include "Server.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffServerBase"



namespace api::onoff {



Server::Server( const base::ServiceName& role_name, const bool exported )
   : m_role_name( role_name )
   , m_exported( exported )
   , m_comm_type( m_exported ? base::eCommType::IPC : base::eCommType::ITC )
{
   // DBG_TRC( "Created" );
   data::OnOffEvent::Event::set_notification( this, m_role_name, eOnOff::RequestTriggerState );
}

Server::~Server( )
{
   // DBG_TRC( "Destroyed" );
   data::OnOffEvent::Event::clear_all_notifications( this );
}

void Server::response_trigger_state( const bool result )
{
   DBG_TRC( "result: %s", BOOL_TO_STRING( result ) );

   create_send_event< data::ResponseTriggerStateData >( eOnOff::ResponseTriggerState, result );
}

void Server::process_event( const data::OnOffEvent::Event& event )
{
   DBG_TRC( "id = %s", to_string( event.info( ).id( ) ).c_str( ) );
   switch( event.info( ).id( ) )
   {
      case eOnOff::RequestTriggerState:
      {
         const data::RequestTriggerStateData* data = get_event_data< data::RequestTriggerStateData >( event );
         request_trigger_state( data->state, data->delay );
         break;
      }
      default: break;
   }
}



} // namespace api::onoff
