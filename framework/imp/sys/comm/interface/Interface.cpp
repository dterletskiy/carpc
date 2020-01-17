#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/comm/interface/Interface.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Interface"



namespace base {



Interface::Interface( const std::string& name, const std::string& role, const eType type )
   : m_name( name )
   , m_role( role )
   , m_type( type )
{
   SYS_TRC( "created" );
   mp_service = ServiceProcess::instance( )->current_service( );
}

Interface::~Interface( )
{
   SYS_TRC( "destroyed" );
}



} // namespace base
