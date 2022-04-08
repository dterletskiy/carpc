// Framework
// Application
#include "imp/app/components/OnOff/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOff"



using namespace application::components::onoff;



carpc::application::IComponent::tSptr Component::creator( )
{
   return std::shared_ptr< Component >( new Component( "OnOff" ) );
}

Component::Component( const std::string& _name )
   : carpc::application::RootComponent( _name )
{
   MSG_DBG( "Created: %s", name( ).c_str( ) );
}

Component::~Component( )
{
   MSG_DBG( "Destroyed: %s", name( ).c_str( ) );
}

void Component::process_boot( const std::string& command )
{
   MSG_DBG( "%s", command.c_str( ) );

   mp_client_onoff_100 = std::make_shared< client::V1_0_0::OnOff >( "rolename" );
   mp_client_onoff_200 = std::make_shared< client::V2_0_0::OnOff >( "rolename" );
}

void Component::process_shutdown( carpc::callback::tBlocker blocker )
{
   MSG_DBG( );
}
