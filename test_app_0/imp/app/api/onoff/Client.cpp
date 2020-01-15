// Application
#include "imp/app/api/onoff/Client.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffClientBase"



namespace api::onoff {



Client::Client( )
{
   // DBG_MSG( "Created" );
   OnOffEvent::Event::set_notification( true, this );
}

Client::~Client( )
{
   // DBG_MSG( "Destroyed" );
   OnOffEvent::Event::set_notification( false, this );
}

void Client::request_trigger_state( const std::string& state )
{
   // DBG_MSG( "state: %s", state.c_str( ) );

   OnOffEvent::Data data( std::make_shared< RequestTriggerStateData >( state ) );
   OnOffEvent::Event::create_send(
           data
         , eOnOff::RequestTriggerState
         , base::eCommType::ITC
      );
}

void Client::process_event( const OnOffEvent::Event& event )
{
   // DBG_MSG( "id = %s", to_string( event.id( ) ).c_str( ) );
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
