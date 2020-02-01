#include "api/sys/service/ServiceThread.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/comm/interface/InterfaceRegistry.hpp"
#include "api/sys/comm/interface/Server.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "BaseServer"



namespace base {



Server::Server( const std::string& name, const std::string& role )
   : Interface( name, role, Interface::eType::server )
{
   SYS_TRC( "created" );
   register_server( this );
}

Server::~Server( )
{
   SYS_TRC( "destroyed" );
   unregister_server( this );
}



} // namespace base
