#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/comm/interface/InterfaceRegistry.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "InterfaceRegistry"



namespace base {



InterfaceRegistry::InterfaceRegistry( )
{
   SYS_TRC( "created" );
}

InterfaceRegistry::~InterfaceRegistry( )
{
   SYS_TRC( "destroyed" );
}



} // namespace base
