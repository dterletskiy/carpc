#include "service/Service.hpp"
#include "tools/Tools.hpp"
#include "Component.hpp"

#include "Trace.hpp"
#define CLASS_ABBR "Component"


namespace base {



Component::Component( const IServicePtr p_service, const std::string& name )
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

IServicePtr Component::service( ) const
{
   if( IServicePtr p_service = mp_service.lock() )
      return p_service;
   else
      return InvalidServicePtr;
}


} // namespace base
