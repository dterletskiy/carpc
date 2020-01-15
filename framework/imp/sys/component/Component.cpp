#include "api/sys/service/Service.hpp"
#include "api/sys/tools/Tools.hpp"
#include "api/sys/component/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Component"


namespace base {



Component::Component( const ServicePtr p_service, const std::string& name )
   : m_name( name )
   , m_id( tools::generate_id( "Component" ) )
   , mp_service( p_service )
   , m_performance( m_name )
{
   DBG_MSG( "Created: %s", m_name.c_str( ) );
}

Component::~Component( )
{
   DBG_MSG( "Destroyed: %s", m_name.c_str( ) );
}

ServicePtr Component::service( ) const
{
   if( ServicePtr p_service = mp_service.lock() )
      return p_service;
   else
      return InvalidServicePtr;
}


} // namespace base
