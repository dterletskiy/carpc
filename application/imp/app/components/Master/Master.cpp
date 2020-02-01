// Application
#include "imp/app/components/Master/Master.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Master"



namespace application::components {



base::ComponentPtr Master::creator( base::ServiceThreadPtr p_service )
{
   return std::shared_ptr< Master >( new Master( p_service, "Master" ) );
}

Master::Master( const base::ServiceThreadPtr p_service, const std::string& name )
   : base::Component( p_service, name )
{
   DBG_MSG( "Created: %s", base::Component::name( ).c_str( ) );
}

Master::~Master( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
}



} // namespace application::components
