// Application
#include "imp/app/components/Driver/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Driver"



namespace application::components::driver {



base::IComponent::tSptr Component::creator( base::IServiceThread::tSptr p_service )
{
   return std::shared_ptr< Component >( new Component( p_service, "Driver" ) );
}

Component::Component( const base::IServiceThread::tSptr p_service, const std::string& name )
   : base::Component( p_service, name )
{
   DBG_MSG( "Created: %s", base::Component::name( ).c_str( ) );
   events::AppEvent::Event::set_notification( this, events::eAppEventID::BOOT );
   events::AppEvent::Event::set_notification( this, events::eAppEventID::SHUTDOWN );
   events::AppEvent::Event::set_notification( this, events::eAppEventID::PING );
}

Component::~Component( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
   events::AppEvent::Event::clear_all_notifications( this );
}

void Component::process_event( const events::AppEvent::Event& event )
{
   DBG_MSG( "message = %s", event.data( )->message.c_str( ) );

   switch( event.info( ).id( ) )
   {
      case events::eAppEventID::BOOT:
      {
         if( nullptr == mp_client_onoff )
         {
            auto request = [ this ]( ){ mp_client_onoff->request_trigger_state( "Driver", 10000000000 ); };
            auto request_start = [ this ]( ){ mp_client_onoff->request_start( ); };
            mp_client_onoff = new clients::onoff::Client( "OnOffService", "OnOffService-Client-Driver", request );
         }
         break;
      }
      default: break;
   }
}



} // namespace application::components::driver
