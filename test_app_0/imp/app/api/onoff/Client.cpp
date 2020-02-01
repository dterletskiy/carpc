// Application
#include "Client.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffClientBase"



namespace api::onoff {



Client::Client( const std::string& role_name, const bool exported )
   : base::Client( interface_name, role_name )
   , m_exported( exported )
   , m_comm_type( m_exported ? base::eCommType::IPC : base::eCommType::ITC )
{
   // DBG_TRC( "Created" );
}

Client::~Client( )
{
   // DBG_TRC( "Destroyed" );
   data::OnOffEvent::Event::clear_all_notifications( this );
}

void Client::connected( const base::Interface* const p_server ) const
{
   DBG_MSG( "server connected: %p", p_server );
}

void Client::disconnected( const base::Interface* const p_server ) const
{
   DBG_MSG( "server disconnected: %p", p_server );
}

void Client::request_trigger_state( const std::string& state, const size_t delay )
{
   DBG_TRC( "state: %s", state.c_str( ) );

   data::OnOffEvent::Event::set_notification( this, role( ), eOnOff::ResponseTriggerState );

   create_send_event< data::RequestTriggerStateData >( eOnOff::RequestTriggerState, state, delay );
}

void Client::process_event( const data::OnOffEvent::Event& event )
{
   DBG_TRC( "id = %s", to_string( event.info( ).id( ) ).c_str( ) );
   switch( event.info( ).id( ) )
   {
      case eOnOff::ResponseTriggerState:
      {
         data::OnOffEvent::Event::clear_notification( this, role( ), eOnOff::ResponseTriggerState );

         const data::ResponseTriggerStateData* data = get_event_data< data::ResponseTriggerStateData >( event );
         response_trigger_state( data->result );
         break;
      }
      default: break;
   }
}



} // namespace api::onoff
