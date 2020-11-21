#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/events/Events.hpp"
#include "api/sys/comm/service/Registry.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Registry"



namespace ev_i = base::events::service;
using namespace base::service;



namespace {

   base::os::Mutex s_mutex;

   struct Connection
   {
      Address        server;
      Address::tSet  clients;
   };
   std::map< Signature, Connection > m_db;

}



const Address& Registry::server( const Signature& signature ) const
{
   static Address dummy;

   const auto iterator = m_db.find( signature );
   if( m_db.end( ) == iterator )
      return dummy;

   return iterator->second.server;
}

const Address::tSet& Registry::clients( const Signature& signature ) const
{
   static Address::tSet dummy;

   const auto iterator = m_db.find( signature );
   if( m_db.end( ) == iterator )
      return dummy;

   return iterator->second.clients;
}

const Address& Registry::server( const Passport& passport ) const
{
   return server( passport.signature );
}

const Address::tSet& Registry::clients( const Passport& passport ) const
{
   return clients( passport.signature );
}

Registry::eResult Registry::register_server( const Signature& signature, const Address& address )
{
   os::MutexAutoLocker locker( s_mutex );

   // Add signature to DB if it is not exists
   auto iterator = m_db.find( signature );
   if( m_db.end( ) == iterator )
   {
      iterator = m_db.emplace( signature, Connection{ } ).first;
   }
   auto& connection = iterator->second;

   // Check is server with current signature already registered
   if( connection.server.is_valid( ) )
   {
      SYS_ERR( "server already registered: %s(%s)", signature.name( ).c_str( ), connection.server.name( ).c_str( ) );
      return eResult::Duplicate;
   }

   // Add server information to DB
   connection.server = address;

   // Check if any client registered with current signature
   if( true == connection.clients.empty( ) )
   {
      SYS_INF( "registered not paired server: %s", signature.name( ).c_str( ) );
      return eResult::OK_NotPaired;
   }
   SYS_INF( "registered paired server: %s", signature.name( ).c_str( ) );

   // Send notification event to clients about registered server with current signature
   ev_i::Status::Event::create_send( { signature, ev_i::eStatus::ServerConnected }, ev_i::Status::Data( address ) );

   // Send notification events to server about each registered client with current signature
   for( const auto& address_client : connection.clients )
      ev_i::Status::Event::create_send( { signature, ev_i::eStatus::ClientConnected }, address_client );

   return eResult::OK_Paired;
}

Registry::eResult Registry::unregister_server( const Signature& signature, const Address& address )
{
   os::MutexAutoLocker locker( s_mutex );

   // Find registered signature in DB
   auto iterator = m_db.find( signature );
   if( m_db.end( ) == iterator )
   {
      SYS_ERR( "signature was not found: %s", signature.name( ).c_str( ) );
      return eResult::Error;
   }
   auto& connection = iterator->second;

   // Check is server with current signature already registered
   if( false == connection.server.is_valid( ) )
   {
      SYS_ERR( "server was not registered: %s(%s)", signature.name( ).c_str( ), address.name( ).c_str( ) );
      return eResult::NotFound;
   }

   // Remove server information from DB
   connection.server = { };

   // Check if any client registered with current signature
   if( true == connection.clients.empty( ) )
   {
      SYS_INF( "unregistered not paired server: %s", signature.name( ).c_str( ) );
      return eResult::OK_NotPaired;
   }
   SYS_INF( "unregistered paired server: %s", signature.name( ).c_str( ) );

   // Send notification event to clients about registered server with current signature
   ev_i::Status::Event::create_send( { signature, ev_i::eStatus::ServerDisconnected }, address );

   return eResult::OK_Paired;
}

Registry::eResult Registry::register_client( const Signature& signature, const Address& address )
{
   os::MutexAutoLocker locker( s_mutex );

   // Add signature to DB if it is not exists
   auto iterator = m_db.find( signature );
   if( m_db.end( ) == iterator )
   {
      iterator = m_db.emplace( signature, Connection{ } ).first;
   }
   auto& connection = iterator->second;

   // Add client information to DB
   auto result = connection.clients.emplace( address );
   if( false == result.second )
   {
      if( connection.clients.end( ) != result.first )
      {
         SYS_ERR( "client already registered %s(%s)", signature.name( ).c_str( ), result.first->name( ).c_str( ) );
         return eResult::Duplicate;
      }

      SYS_ERR( "unable register client %s", signature.name( ).c_str( ) );
      return eResult::Error;
   }

   // Check if any server already registered with current signature
   if( false == connection.server.is_valid( ) )
   {
      SYS_INF( "registered not paired client: %s", signature.name( ).c_str( ) );
      return eResult::OK_NotPaired;
   }
   SYS_INF( "registered paired client: %s", signature.name( ).c_str( ) );

   // Send notification event to server about registered client with current signature
   ev_i::Status::Event::create_send( { signature, ev_i::eStatus::ClientConnected }, address );
   // Send notification events to client about registered server with current signature
   ev_i::Status::Event::create_send( { signature, ev_i::eStatus::ServerConnected }, connection.server );

   return eResult::OK_Paired;
}

Registry::eResult Registry::unregister_client( const Signature& signature, const Address& address )
{
   os::MutexAutoLocker locker( s_mutex );

   // Find registered signature in DB
   auto iterator = m_db.find( signature );
   if( m_db.end( ) == iterator )
   {
      SYS_ERR( "signature was not found: %s", signature.name( ).c_str( ) );
      return eResult::Error;
   }
   auto& connection = iterator->second;

   // Find registered client with current signature and remove it if exists
   const size_t result = connection.clients.erase( address );
   // Check if any client was removed
   if( 0 == result )
   {
      SYS_ERR( "client was not registered: %s(%s)", signature.name( ).c_str( ), address.name( ).c_str( ) );
      return eResult::NotFound;
   }

   // Check if server registered with current signature
   if( false == connection.server.is_valid( ) )
   {
      SYS_INF( "unregistered not paired client: %s", signature.name( ).c_str( ) );
      return eResult::OK_NotPaired;
   }
   SYS_INF( "unregistered paired client: %s", signature.name( ).c_str( ) );

   // Send notification event about unregistered client with current signature
   ev_i::Status::Event::create_send( { signature, ev_i::eStatus::ClientDisconnected }, address );

   return eResult::OK_Paired;
}

Registry::eResult Registry::register_server( const Passport& passport )
{
   return register_server( passport.signature, passport.address );
}

Registry::eResult Registry::unregister_server( const Passport& passport )
{
   return unregister_server( passport.signature, passport.address );
}

Registry::eResult Registry::register_client( const Passport& passport )
{
   return register_client( passport.signature, passport.address );
}

Registry::eResult Registry::unregister_client( const Passport& passport )
{
   return unregister_client( passport.signature, passport.address );
}
