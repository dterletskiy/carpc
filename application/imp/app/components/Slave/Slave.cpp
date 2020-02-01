// Application
#include "imp/app/components/Slave/Slave.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Slave"



namespace application::components {



base::ComponentPtr Slave::creator( base::ServiceThreadPtr p_service )
{
   return std::shared_ptr< Slave >( new Slave( p_service, "Slave" ) );
}

Slave::Slave( const base::ServiceThreadPtr p_service, const std::string& name )
   : base::Component( p_service, name )
{
   DBG_MSG( "Created: %s", base::Component::name( ).c_str( ) );
}

Slave::~Slave( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
}



} // namespace application::components
