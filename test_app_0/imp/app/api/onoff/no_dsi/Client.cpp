// Application
#include "Client.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffClientBase"



namespace api::onoff::no_dsi {



Client::Client( const base::ServiceName& service_name )
   : m_service_name( service_name )
{
   // DBG_TRC( "Created" );
}

Client::~Client( )
{
   // DBG_TRC( "Destroyed" );
   OnOffEvent::Event::clear_all_notifications( this );
}

void Client::request_trigger_state( const std::string& state, const size_t delay )
{
   DBG_TRC( "state: %s", state.c_str( ) );

   OnOffEvent::Event::set_notification( this, m_service_name, eOnOff::ResponseTriggerState );

   OnOffEvent::Data data( std::make_shared< RequestTriggerStateData >( state, delay ) );
   OnOffEvent::Event::create_send(
           m_service_name
         , eOnOff::RequestTriggerState
         , data
         , base::eCommType::ITC
      );
}

void Client::process_event( const OnOffEvent::Event& event )
{
   DBG_TRC( "id = %s", to_string( event.info( ).id( ) ).c_str( ) );
   switch( event.info( ).id( ) )
   {
      case eOnOff::ResponseTriggerState:
      {
         OnOffEvent::Event::clear_notification( this, m_service_name, eOnOff::ResponseTriggerState );

         ResponseTriggerStateData* data = static_cast< ResponseTriggerStateData* >( event.data( )->ptr.get( ) );
         response_trigger_state( data->result );
         break;
      }
      default: break;
   }
}



} // namespace api::onoff::no_dsi
