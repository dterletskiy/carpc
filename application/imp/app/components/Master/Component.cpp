// Application
#include "imp/app/components/Master/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Master"



using namespace application::components::master;



base::component::IComponent::tSptr Component::creator( base::application::IThread& service )
{
   return std::shared_ptr< Component >( new Component( service, "Master" ) );
}

Component::Component( base::application::IThread& _service, const std::string& _name )
   : base::component::Component( _service, _name )
{
   MSG_DBG( "Created: %s", name( ).c_str( ) );
   events::AppEvent::Event::set_notification( this, events::eAppEventID::BOOT );
   events::AppEvent::Event::set_notification( this, events::eAppEventID::SHUTDOWN );
   events::AppEvent::Event::set_notification( this, events::eAppEventID::PING );
}

Component::~Component( )
{
   MSG_DBG( "Destroyed: %s", name( ).c_str( ) );
   events::AppEvent::Event::clear_all_notifications( this );
}

void Component::process_event( const events::AppEvent::Event& event )
{
   MSG_DBG( "message = %s", event.data( )->message.c_str( ) );

   switch( event.info( ).id( ) )
   {
      case events::eAppEventID::BOOT:
      {
         if( nullptr == mp_client_onoff )
         {
            auto request = [ this ]( ){ mp_client_onoff->request_trigger_state( "Master", 5000000000 ); };
            auto request_start = [ this ]( ){ mp_client_onoff->request_start( ); };
            mp_client_onoff = std::shared_ptr< clients::onoff::Client >(
               new clients::onoff::Client( "OnOffService", "OnOffService-Client-Master", request )
            );
         }
         break;
      }
      default: break;
   }
}
