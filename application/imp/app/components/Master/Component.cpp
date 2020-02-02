// Application
#include "imp/app/components/Master/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Master"



namespace application::components::master {



base::ComponentPtr Component::creator( base::ServiceThreadPtr p_service )
{
   return std::shared_ptr< Component >( new Component( p_service, "Master" ) );
}

Component::Component( const base::ServiceThreadPtr p_service, const std::string& name )
   : base::Component( p_service, name )
{
   DBG_MSG( "Created: %s", base::Component::name( ).c_str( ) );
}

Component::~Component( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
}



} // namespace application::components::master
