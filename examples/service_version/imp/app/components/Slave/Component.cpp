// Application
#include "imp/app/components/Slave/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Slave"



using namespace application::components::slave;



carpc::application::IComponent::tSptr Component::creator( )
{
   return std::shared_ptr< Component >( new Component( "Slave" ) );
}

Component::Component( const std::string& _name )
   : carpc::application::Component( _name )
   , m_onoff_server( "rolename" )
{
   MSG_DBG( "Created: %s", name( ).c_str( ) );
}

Component::~Component( )
{
   MSG_DBG( "Destroyed: %s", name( ).c_str( ) );
}
