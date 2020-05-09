#include "api/sys/tools/Tools.hpp"
#include "api/sys/component/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Component"


using namespace base;



Component::Component( IServiceThread& service, const std::string& name )
   : m_name( name )
   , m_id( tools::generate_id( "Component" ) )
   , m_service( service )
   , m_performance( m_name )
{
   DBG_MSG( "Created: %s", m_name.c_str( ) );
}

Component::~Component( )
{
   DBG_MSG( "Destroyed: %s", m_name.c_str( ) );
}
