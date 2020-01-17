#include "api/sys/comm/interface/Registry.hpp"
#include "api/sys/comm/interface/Server.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Server"



namespace base {



Server::Server( const std::string& name, const std::string& role )
   : Interface( name, role, Interface::eType::server )
{
   SYS_TRC( "created" );
   Registry::instance( )->register_server( this );
}

Server::~Server( )
{
   SYS_TRC( "destroyed" );
   Registry::instance( )->unregister_server( this );
}



} // namespace base
