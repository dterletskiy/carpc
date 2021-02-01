#include "api/sys/tools/Tools.hpp"
#include "api/sys/component/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Component"


using namespace base::component;



Component::Component( application::IThread& app_thread, const std::string& name )
   : m_name( name )
   , m_app_thread( app_thread )
   , m_performance( m_name )
{
   MSG_DBG( "Created: %s", m_name.c_str( ) );
}

Component::~Component( )
{
   MSG_DBG( "Destroyed: %s", m_name.c_str( ) );
}
