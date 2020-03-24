// Application
#include "imp/app/components/Driver/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Driver"



namespace application::components::driver {



base::ComponentPtr Component::creator( base::ServiceThreadPtr p_service )
{
   return std::shared_ptr< Component >( new Component( p_service, "Driver" ) );
}

Component::Component( const base::ServiceThreadPtr p_service, const std::string& name )
   : base::Component( p_service, name )
{
   DBG_MSG( "Created: %s", base::Component::name( ).c_str( ) );
   events::ID::PingEvent::Event::set_notification( this, events::eEventID::boot );
   events::ID::PingEvent::Event::set_notification( this, events::eEventID::ping );
}

Component::~Component( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
   events::ID::PingEvent::Event::clear_all_notifications( this );
}

void Component::process_event( const events::ID::PingEvent::Event& event )
{
   DBG_MSG( "message = %s", event.data( )->message.c_str( ) );

   switch( event.info( ).id( ) )
   {
      case events::eEventID::boot:
      {
         if( nullptr == mp_client_onoff )
         {
            auto request = [ this ]( ){ mp_client_onoff->request_trigger_state( "Driver", 10000000000 ); };
            auto request_start = [ this ]( ){ mp_client_onoff->request_start( ); };
            mp_client_onoff = new clients::onoff::Client( "OnOffService", "OnOffService-Client-Driver", request_start );
         }
         break;
      }
      default: break;
   }
}



} // namespace application::components::driver
