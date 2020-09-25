#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/events/Events.hpp"
#include "api/sys/comm/interface/ConnectionDB.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "ConnectionDB"



namespace ev_i = base::events::interface;
using namespace base::interface;



namespace { base::os::Mutex s_mutex; }



const Address::tOpt& ConnectionDB::server( const Signature& signature ) const
{
   static Address::tOpt dummy = std::nullopt;

   const auto iterator = m_db.find( signature );
   if( m_db.end( ) == iterator )
      return dummy;

   return iterator->second.server;
}

const Address::tSet& ConnectionDB::clients( const Signature& signature ) const
{
   static Address::tSet dummy = { };

   const auto iterator = m_db.find( signature );
   if( m_db.end( ) == iterator )
      return dummy;

   return iterator->second.clients;
}

ConnectionDB::eResult ConnectionDB::register_server( const Signature& signature, const Address& address )
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
   if( std::nullopt != connection.server )
   {
      SYS_ERR( "server already registered: %s(%p)", signature.name( ).c_str( ), connection.server.value( ).ptr( ) );
      return eResult::Duplicate;
   }

   // Add server information to DB
   connection.server = address;

   // Check if any client registered with current signature
   if( true == connection.clients.empty( ) )
      return eResult::OK_NotPaired;

   // Send notification event to clients about registered server with current signature
   ev_i::Status::Event::create_send( { signature, ev_i::eStatus::ServerConnected }, address, eCommType::ITC );

   // Send notification events to server about each registered client with current signature
   for( const auto& address_client : connection.clients )
      ev_i::Status::Event::create_send( { signature, ev_i::eStatus::ClientConnected }, address_client, eCommType::ITC );

   return eResult::OK_Paired;
}

ConnectionDB::eResult ConnectionDB::unregister_server( const Signature& signature, const Address& address )
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
   if( std::nullopt == connection.server )
   {
      SYS_ERR( "server was not registered: %s(%p)", signature.name( ).c_str( ), address.ptr( ) );
      return eResult::NotFound;
   }

   // Remove server information from DB
   connection.server.reset( );

   // Check if any client registered with current signature
   if( true == connection.clients.empty( ) )
      return eResult::OK_NotPaired;

   // Send notification event to clients about registered server with current signature
   ev_i::Status::Event::create_send( { signature, ev_i::eStatus::ServerDisconnected }, address, eCommType::ITC );

   return eResult::OK_Paired;
}

ConnectionDB::eResult ConnectionDB::register_client( const Signature& signature, const Address& address )
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
         SYS_ERR( "client already registered %s(%p)", signature.name( ).c_str( ), result.first->ptr( ) );
         return eResult::Duplicate;
      }

      SYS_ERR( "unable register client %s", signature.name( ).c_str( ) );
      return eResult::Error;
   }

   // Check if any server already registered with current signature
   if( std::nullopt == connection.server )
      return eResult::OK_NotPaired;

   // Send notification event to server about registered client with current signature
   ev_i::Status::Event::create_send( { signature, ev_i::eStatus::ClientConnected }, address, eCommType::ITC );
   // Send notification events to client about registered server with current signature
   ev_i::Status::Event::create_send( { signature, ev_i::eStatus::ServerConnected }, connection.server.value( ), eCommType::ITC );

   return eResult::OK_Paired;
}

ConnectionDB::eResult ConnectionDB::unregister_client( const Signature& signature, const Address& address )
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
      SYS_ERR( "client was not registered: %s(%p)", signature.name( ).c_str( ), address.ptr( ) );
      return eResult::NotFound;
   }

   // Check if server registered with current signature
   if( std::nullopt == connection.server )
      return eResult::OK_NotPaired;

   // Send notification event about unregistered client with current signature
   ev_i::Status::Event::create_send( { signature, ev_i::eStatus::ClientDisconnected }, address, eCommType::ITC );

   return eResult::OK_Paired;
}
