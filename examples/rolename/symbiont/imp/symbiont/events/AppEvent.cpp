#include "AppEvent.hpp"




namespace application::events {

   const char* c_str( const eAppEventID id )
   {
      switch( id )
      {
         case eAppEventID::BOOT:       return "BOOT";
         case eAppEventID::SHUTDOWN:   return "SHUTDOWN";
         case eAppEventID::PING:       return "PING";
         case eAppEventID::UNDEFINED:  return "UNDEFINED";
         default:                      return "UNDEFINED";
      }
   }

} // namespace application::events
