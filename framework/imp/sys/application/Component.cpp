#include "api/sys/application/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Component"


using namespace base::application;



Component::Component( const std::string& name )
   : IComponent( name )
{
   MSG_DBG( "Created: %s", m_name.c_str( ) );
}

Component::~Component( )
{
   MSG_DBG( "Destroyed: %s", m_name.c_str( ) );
}
