#include "api/sys/events/Events.hpp"



namespace base::events::service {

   const char* c_str( const eID id )
   {
      switch( id )
      {
         case eID::boot:         return "base::events::service::eID::boot";
         case eID::shutdown:     return "base::events::service::eID::shutdown";
         case eID::ping:         return "base::events::service::eID::ping";
         case eID::undefined:
         default:                return "base::events::service::eID::undefined";
      }
   }

}



namespace base::events::interface {

   const char* c_str( const eAction id )
   {
      switch( id )
      {
         case eAction::RegisterServer:       return "base::events::interface::eAction::RegisterServer";
         case eAction::UnregisterServer:     return "base::events::interface::eAction::UnregisterServer";
         case eAction::RegisterClient:       return "base::events::interface::eAction::RegisterClient";
         case eAction::UnregisterClient:     return "base::events::interface::eAction::UnregisterClient";
         case eAction::Undefined:
         default:                            return "base::events::interface::eAction::Undefined";
      }
   }

   bool ActionData::to_stream( base::dsi::tByteStream& stream ) const
   {
      return stream.push( signature, ptr );
   }

   bool ActionData::from_stream( base::dsi::tByteStream& stream )
   {
      return stream.pop( ptr, signature );
   }



   const char* c_str( const eStatus id )
   {
      switch( id )
      {
         case eStatus::ServerConnected:      return "base::events::interface::eStatus::ServerConnected";
         case eStatus::ServerDisconnected:   return "base::events::interface::eStatus::ServerDisconnected";
         case eStatus::ClientConnected:      return "base::events::interface::eStatus::ClientConnected";
         case eStatus::ClientDisconnected:   return "base::events::interface::eStatus::ClientDisconnected";
         case eStatus::Undefined:
         default:                            return "base::events::interface::eStatus::Undefined";
      }
   }


   SignatureStatus::SignatureStatus( const base::interface::Signature& signature, const eStatus id )
      : m_signature( signature )
      , m_id( id )
   { }

   SignatureStatus::SignatureStatus( const SignatureStatus& other )
      : m_signature( other.m_signature )
      , m_id( other.m_id )
   { }

   const std::string SignatureStatus::name( ) const
   {
      return base::format_string(
              ", signature: "    , m_signature.name( )
            , ", id: "           , size_t(m_id)
         );
   }

   bool SignatureStatus::operator<( const SignatureStatus& other ) const
   {
      if( other.m_signature != m_signature )
         return m_signature < other.m_signature;

      return m_id < other.m_id;
   }

   const bool SignatureStatus::to_stream( dsi::tByteStream& stream ) const
   {
      return stream.push( m_signature, m_id );
   }

   const bool SignatureStatus::from_stream( dsi::tByteStream& stream )
   {
      return stream.pop( m_id, m_signature );
   }

   const base::interface::Signature& SignatureStatus::signature( ) const
   {
      return m_signature;
   }

   const eStatus SignatureStatus::id( ) const
   {
      return m_id;
   }

} // namespase base