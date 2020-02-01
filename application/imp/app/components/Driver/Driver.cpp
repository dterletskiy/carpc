// Application
#include "imp/app/components/Driver/Driver.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Driver"



namespace application::components {



base::ComponentPtr Driver::creator( base::ServiceThreadPtr p_service )
{
   return std::shared_ptr< Driver >( new Driver( p_service, "Driver" ) );
}

Driver::Driver( const base::ServiceThreadPtr p_service, const std::string& name )
   : base::Component( p_service, name )
{
   DBG_MSG( "Created: %s", base::Component::name( ).c_str( ) );
}

Driver::~Driver( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
}



} // namespace application::components
