#include "api/sys/service/ServiceThread.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/comm/interface/InterfaceRegistry.hpp"
#include "api/sys/comm/interface/Client.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "BaseClient"



namespace base {



Client::Client( const std::string& name, const std::string& role, const bool is_ipc )
   : Interface( name, role, is_ipc, Interface::eType::client )
{
   SYS_TRC( "created" );
   register_client( this );
}

Client::~Client( )
{
   SYS_TRC( "destroyed" );
   unregister_client( this );
}



} // namespace base
