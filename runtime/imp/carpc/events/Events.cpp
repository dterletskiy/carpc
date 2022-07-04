#include "carpc/events/Events.hpp"



namespace carpc::events::system {

   const char* c_str( const eID id )
   {
      switch( id )
      {
         case eID::boot:         return "carpc::events::system::eID::boot";
         case eID::shutdown:     return "carpc::events::system::eID::shutdown";
         case eID::ping:         return "carpc::events::system::eID::ping";
         case eID::undefined:
         default:                return "carpc::events::system::eID::undefined";
      }
   }

}



namespace carpc::events::service {

   const char* c_str( const eStatus id )
   {
      switch( id )
      {
         case eStatus::ServerConnected:      return "carpc::events::service::eStatus::ServerConnected";
         case eStatus::ServerDisconnected:   return "carpc::events::service::eStatus::ServerDisconnected";
         case eStatus::ClientConnected:      return "carpc::events::service::eStatus::ClientConnected";
         case eStatus::ClientDisconnected:   return "carpc::events::service::eStatus::ClientDisconnected";
         case eStatus::Undefined:
         default:                            return "carpc::events::service::eStatus::Undefined";
      }
   }


   SignatureStatus::SignatureStatus( const carpc::service::Signature& signature, const eStatus id )
      : m_signature( signature )
      , m_id( id )
   {
   }

   SignatureStatus::SignatureStatus( const SignatureStatus& other )
      : m_signature( other.m_signature )
      , m_id( other.m_id )
   {
   }

   const std::string SignatureStatus::dbg_name( ) const
   {
      return carpc::format_string( "signature: ", m_signature.dbg_name( ), ", id: ", size_t(m_id) );
   }

   bool SignatureStatus::operator<( const SignatureStatus& other ) const
   {
      if( other.m_signature != m_signature )
         return m_signature < other.m_signature;

      return m_id < other.m_id;
   }

   const bool SignatureStatus::to_stream( ipc::tStream& stream ) const
   {
      return ipc::serialize( stream, m_signature, m_id );
   }

   const bool SignatureStatus::from_stream( ipc::tStream& stream )
   {
      return ipc::deserialize( stream, m_signature, m_id );
   }

   const carpc::service::Signature& SignatureStatus::signature( ) const
   {
      return m_signature;
   }

   const eStatus SignatureStatus::id( ) const
   {
      return m_id;
   }

}



namespace carpc::events::service {

   const char* c_str( const eAction id )
   {
      switch( id )
      {
         case eAction::RegisterServer:       return "carpc::events::interface::eAction::RegisterServer";
         case eAction::UnregisterServer:     return "carpc::events::interface::eAction::UnregisterServer";
         case eAction::RegisterClient:       return "carpc::events::interface::eAction::RegisterClient";
         case eAction::UnregisterClient:     return "carpc::events::interface::eAction::UnregisterClient";
         case eAction::Undefined:
         default:                            return "carpc::events::interface::eAction::Undefined";
      }
   }

}