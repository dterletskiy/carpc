#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/comm/interface/Server.hpp"
#include "api/sys/comm/interface/Client.hpp"
#include "api/sys/comm/interface/Registry.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Registry"



namespace base {



Registry::Registry( )
{
   SYS_TRC( "created" );
}

Registry::~Registry( )
{
   SYS_TRC( "destroyed" );
}

RegistryPtr Registry::mp_instance;
RegistryPtr Registry::instance( )
{
   os::Mutex mutex( true );
   if( !mp_instance )
   {
      mp_instance.reset( new Registry( ) );
   }

   return mp_instance;
}

bool Registry::register_server( const Server* const p_server )
{
   auto iterator = m_server_registry.find( p_server );
   if( m_server_registry.end( ) != iterator )
   {
      SYS_WRN( "Server already registered: %p", p_server );
      return false;
   }
   m_server_registry.emplace( p_server );

   for( auto iterator = m_client_registry.begin( ); iterator != m_client_registry.end( ); ++iterator )
   {
      const Client* p_client = *iterator;
      if( ( p_server->name( ) == p_client->name( ) ) && ( p_server->role( ) == p_client->role( ) ) )
      {
         SYS_TRC( "Found registered client: %p", p_client );
         p_client->connected( );
         p_server->connected( );
      }
   }

   return true;
}

bool Registry::register_client( const Client* const p_client )
{
   auto iterator = m_client_registry.find( p_client );
   if( m_client_registry.end( ) != iterator )
   {
      SYS_WRN( "Client already registered: %p", p_client );
      return false;
   }
   m_client_registry.emplace( p_client );

   for( auto iterator = m_server_registry.begin( ); iterator != m_server_registry.end( ); ++iterator )
   {
      const Server* p_server = *iterator;
      if( ( p_client->name( ) == p_server->name( ) ) && ( p_client->role( ) == p_server->role( ) ) )
      {
         SYS_TRC( "Found registered server: %p", p_server );
         p_server->connected( );
         p_client->connected( );
         break;
      }
   }

   return true;
}

bool Registry::unregister_server( const Server* const p_server )
{
   auto iterator = m_server_registry.find( p_server );
   if( m_server_registry.end( ) == iterator )
   {
      SYS_WRN( "Not found registered server: %p", p_server );
      return false;
   }
   m_server_registry.erase( iterator );

   for( auto iterator = m_client_registry.begin( ); iterator != m_client_registry.end( ); ++iterator )
   {
      const Client* p_client = *iterator;
      if( ( p_server->name( ) == p_client->name( ) ) && ( p_server->role( ) == p_client->role( ) ) )
      {
         SYS_TRC( "Found registered client: %p", p_client );
         p_client->disconnected( );
      }
   }

   return true;
}

bool Registry::unregister_client( const Client* const p_client )
{
   auto iterator = m_client_registry.find( p_client );
   if( m_client_registry.end( ) == iterator )
   {
      SYS_WRN( "Not found registered client: %p", p_client );
      return false;
   }
   m_client_registry.erase( iterator );

   for( auto iterator = m_server_registry.begin( ); iterator != m_server_registry.end( ); ++iterator )
   {
      const Server* p_server = *iterator;
      if( ( p_server->name( ) == p_client->name( ) ) && ( p_server->role( ) == p_client->role( ) ) )
      {
         SYS_TRC( "Found registered server: %p", p_server );
         p_server->disconnected( );
         break;
      }
   }

   return true;
}



} // namespace base
