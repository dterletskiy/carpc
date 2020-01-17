#include "api/sys/comm/interface/Registry.hpp"
#include "api/sys/comm/interface/Client.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Client"



namespace base {



Client::Client( const std::string& name, const std::string& role )
   : Interface( name, role, Interface::eType::client )
{
   SYS_TRC( "created" );
   Registry::instance( )->register_client( this );
}

Client::~Client( )
{
   SYS_TRC( "destroyed" );
   Registry::instance( )->unregister_client( this );
}



} // namespace base
