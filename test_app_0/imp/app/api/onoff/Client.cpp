// Application
#include "imp/app/api/onoff/Client.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffClientBase"



namespace api::onoff {



Client::Client( )
{
   // DBG_TRC( "Created" );
   ServiceOnOff::OnOffEvent::Event::set_notification( true, this, eOnOff::ResponseTriggerState );
}

Client::~Client( )
{
   // DBG_TRC( "Destroyed" );
   ServiceOnOff::OnOffEvent::Event::set_notification( false, this, eOnOff::ResponseTriggerState );
}

void Client::request_trigger_state( const std::string& state )
{
   DBG_TRC( "state: %s", state.c_str( ) );

   ServiceOnOff::OnOffEvent::Data data( std::make_shared< RequestTriggerStateData >( state ) );
   ServiceOnOff::OnOffEvent::Event::create_send(
           data
         , eOnOff::RequestTriggerState
         , base::eCommType::IPC
      );
}

void Client::process_event( const ServiceOnOff::OnOffEvent::Event& event )
{
   DBG_TRC( "id = %s", to_string( event.id( ) ).c_str( ) );
   switch( event.id( ) )
   {
      case eOnOff::ResponseTriggerState:
      {
         ResponseTriggerStateData* data = static_cast< ResponseTriggerStateData* >( event.data( )->ptr.get( ) );
         response_trigger_state( data->result );
         break;
      }
      default: break;
   }
}



} // namespace api::onoff
