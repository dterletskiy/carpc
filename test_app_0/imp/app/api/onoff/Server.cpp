// Application
#include "imp/app/api/onoff/Server.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffServerBase"



namespace api::onoff {



Server::Server( )
{
   // DBG_MSG( "Created" );
   OnOffEvent::Event::set_notification( true, this );
}

Server::~Server( )
{
   // DBG_MSG( "Destroyed" );
   OnOffEvent::Event::set_notification( false, this );
}

void Server::response_trigger_state( const bool result )
{
   // DBG_MSG( "result: %s", BOOL_TO_STRING( result ) );

   OnOffEvent::Data data( std::make_shared< ResponseTriggerStateData >( result ) );
   OnOffEvent::Event::create_send(
           data
         , eOnOff::ResponseTriggerState
         , base::eCommType::ITC
      );
}

void Server::process_event( const OnOffEvent::Event& event )
{
   // DBG_MSG( "id = %s", to_string( event.id( ) ).c_str( ) );
   switch( event.id( ) )
   {
      case eOnOff::RequestTriggerState:
      {
         RequestTriggerStateData* data = static_cast< RequestTriggerStateData* >( event.data( )->ptr.get( ) );
         request_trigger_state( data->state );
         break;
      }
      default: break;
   }
}



} // namespace api::onoff
