#include "imp/experimental/components/system/clients/OnOff.hpp"
#include "imp/experimental/components/system/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Component_System"



using namespace component::system;



void OnOffEventConsumer::process( const events::OnOffEvent& event )
{
   MSG_DBG( "id: %zu / message: %s", event.info_t( ).id( ), event.data( ).message.c_str( ) );
}



std::unique_ptr< fw::component::Base > Component::creator( )
{
   return std::make_unique< Component >( );
}

Component::Component( )
   : fw::component::Base( "System" )
{
   mp_onoff_client = clients::OnOff::create( );
   mp_onoff_client->check_in( );

   // mp_onoff_event_consumer = std::make_shared< OnOffEventConsumer >( );
   // events::OnOffEvent::set_notification( mp_onoff_event_consumer, 12345 );
}

Component::~Component( )
{
}
