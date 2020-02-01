#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/comm/event/Event.hpp"
#include "api/sys/comm/interface/Server.hpp"
#include "api/sys/comm/interface/Client.hpp"
#include "api/sys/comm/interface/InterfaceRegistry.hpp"
#include "imp/sys/comm/interface/InterfaceStatusHandler.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "InterfaceRegistry"



namespace base {
   namespace {
      std::set< const Server* > s_server_registry;
      std::set< const Client* > s_client_registry;
      os::Mutex s_mutex;
   }
}



namespace base {



InterfaceRegistryPtr InterfaceRegistry::mp_instance = nullptr;

InterfaceRegistry::InterfaceRegistry( )
{
   SYS_TRC( "created" );
}

InterfaceRegistry::~InterfaceRegistry( )
{
   SYS_TRC( "destroyed" );
}

InterfaceRegistryPtr InterfaceRegistry::instance( )
{
   os::Mutex mutex( true );
   if( !mp_instance )
   {
      mp_instance.reset( new InterfaceRegistry( ) );
   }

   return mp_instance;
}

bool InterfaceRegistry::register_server( const Server* const p_server )
{
   os::MutexAutoLocker mutex( s_mutex );

   for( const Server* p_item : s_server_registry )
   {
      if( true == p_server->cmp_names( p_item ) )
      {
         SYS_WRN( "Server with this inteface name and role name has been already registered: %p", p_item );
         return false;
      }
   }
   s_server_registry.emplace( p_server );
   SYS_TRC( "Server registered: %p", p_server );

   for( const Client* p_client : s_client_registry )
   {
      if( false == p_server->cmp_names( p_client ) )
         continue;

      SYS_TRC( "Found registered client: %p", p_client );
      // Event for server that client was connected to current thread because this code executed in server thread
      InterfaceRegistryEvent::Event::create_send( eInterfaceRegistry::ClientConnected, { p_server, p_client }, eCommType::ETC );
      // Event for client that server was connedted to client context because this code executed in server thread
      InterfaceRegistryEvent::Event::create_send_to_context( eInterfaceRegistry::ServerConnected, { p_server, p_client }, p_client->context( ) );
   }

   return true;
}

bool InterfaceRegistry::register_client( const Client* const p_client )
{
   os::MutexAutoLocker mutex( s_mutex );

   auto iterator = s_client_registry.find( p_client );
   if( s_client_registry.end( ) != iterator )
   {
      SYS_WRN( "Client already registered: %p", p_client );
      return false;
   }
   s_client_registry.emplace( p_client );
   SYS_TRC( "Client registered: %p", p_client );

   for( const Server* p_server : s_server_registry )
   {
      if( false == p_server->cmp_names( p_client ) )
         continue;

      SYS_TRC( "Found registered server: %p", p_server );
      // Event for server that client was connected to current thread because this code executed in server thread
      InterfaceRegistryEvent::Event::create_send_to_context( eInterfaceRegistry::ClientConnected, { p_server, p_client }, p_server->context( ) );
      // Event for client that server was connedted to client context because this code executed in server thread
      InterfaceRegistryEvent::Event::create_send( eInterfaceRegistry::ServerConnected, { p_server, p_client }, eCommType::ETC );
      break;
   }

   return true;
}

bool InterfaceRegistry::unregister_server( const Server* const p_server )
{
   os::MutexAutoLocker mutex( s_mutex );

   auto iterator = s_server_registry.find( p_server );
   if( s_server_registry.end( ) == iterator )
   {
      SYS_WRN( "Not found registered server: %p", p_server );
      return false;
   }
   s_server_registry.erase( iterator );
   SYS_TRC( "Server unregistered: %p", p_server );

   for( const Client* p_client : s_client_registry )
   {
      if( false == p_server->cmp_names( p_client ) )
         continue;

      SYS_TRC( "Found registered client: %p", p_client );
      // Event for client that server was disconnected to client context because this code executed in server thread
      InterfaceRegistryEvent::Event::create_send_to_context( eInterfaceRegistry::ServerConnected, { p_server, p_client }, p_client->context( ) );
   }

   return true;
}

bool InterfaceRegistry::unregister_client( const Client* const p_client )
{
   os::MutexAutoLocker mutex( s_mutex );

   auto iterator = s_client_registry.find( p_client );
   if( s_client_registry.end( ) == iterator )
   {
      SYS_WRN( "Not found registered client: %p", p_client );
      return false;
   }
   s_client_registry.erase( iterator );
   SYS_TRC( "Client unregistered: %p", p_client );

   for( const Server* p_server : s_server_registry )
   {
      if( false == p_server->cmp_names( p_client ) )
         continue;

      SYS_TRC( "Found registered server: %p", p_server );
      // Event for server that client was connected to current thread because this code executed in server thread
      InterfaceRegistryEvent::Event::create_send_to_context( eInterfaceRegistry::ClientConnected, { p_server, p_client }, p_server->context( ) );
      break;
   }

   return true;
}



} // namespace base
