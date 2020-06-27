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

   const char* c_str( const eID id )
   {
      switch( id )
      {
         case eID::ServerConnected:       return "base::events::interface::eID::ServerConnected";
         case eID::ServerDisconnected:    return "base::events::interface::eID::ServerDisconnected";
         case eID::ClientConnected:       return "base::events::interface::eID::ClientConnected";
         case eID::ClientDisconnected:    return "base::events::interface::eID::ClientDisconnected";
         case eID::Undefined:
         default:                         return "base::events::interface::eID::Undefined";
      }
   }

   bool Data::to_stream( base::dsi::tByteStream& stream ) const
   {
      return stream.push( ptr );
   }

   bool Data::from_stream( base::dsi::tByteStream& stream )
   {
      return stream.pop( ptr );
   }

} // namespase base