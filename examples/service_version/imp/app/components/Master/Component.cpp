// Application
#include "imp/app/components/Master/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Master"



using namespace application::components::master;



carpc::application::IComponent::tSptr Component::creator( )
{
   return std::shared_ptr< Component >( new Component( "Master" ) );
}

Component::Component( const std::string& _name )
   : carpc::application::Component( _name )
   , m_server_onoff( "rolename" )
{
   MSG_DBG( "Created: %s", name( ).c_str( ) );
}

Component::~Component( )
{
   MSG_DBG( "Destroyed: %s", name( ).c_str( ) );
}
