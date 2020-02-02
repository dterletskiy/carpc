#include "api/sys/service/ServiceThread.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/comm/interface/InterfaceRegistry.hpp"
#include "api/sys/comm/interface/Server.hpp"
#include "api/sys/comm/interface/Client.hpp"
#include "api/sys/comm/interface/Interface.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "BaseInterface"



namespace base {



Interface::Interface( const std::string& name, const std::string& role, const bool is_ipc, const eType type )
   : m_name( name )
   , m_role( role )
   , m_is_ipc( is_ipc )
   , m_type( type )
{
   SYS_TRC( "created" );
   mp_service_thread = ServiceProcess::instance( )->current_service( );
}

Interface::~Interface( )
{
   SYS_TRC( "destroyed" );
}

const bool Interface::register_server( const Server* const p_server )
{
   if( auto p_interface_registry = ServiceProcess::instance( )->interface_registry( ) )
   {
      return p_interface_registry->register_server( p_server );
   }

   SYS_ERR( "Interface registry goes not exist" );
   return false;
}

const bool Interface::register_client( const Client* const p_client )
{
   if( auto p_interface_registry = ServiceProcess::instance( )->interface_registry( ) )
   {
      return p_interface_registry->register_client( p_client );
   }

   SYS_ERR( "Interface registry goes not exist" );
   return false;
}

const bool Interface::unregister_server( const Server* const p_server )
{
   if( auto p_interface_registry = ServiceProcess::instance( )->interface_registry( ) )
   {
      return p_interface_registry->unregister_server( p_server );
   }

   SYS_ERR( "Interface registry goes not exist" );
   return false;
}

const bool Interface::unregister_client( const Client* const p_client )
{
   if( auto p_interface_registry = ServiceProcess::instance( )->interface_registry( ) )
   {
      return p_interface_registry->unregister_client( p_client );
   }

   SYS_ERR( "Interface registry goes not exist" );
   return false;
}



} // namespace base
