#pragma once

#include <string>
#include <memory>
#include <stdint.h>



namespace base {

   using EventTypeID = std::string;
   using EventInfoID = size_t;

   enum class eCommType : size_t { IPC, ITC, ETC, NONE };
   const char* c_str( const eCommType );

   enum class eDummyEventID : EventInfoID { dummy = SIZE_MAX };
   const char* c_str( const eDummyEventID );

   class Event;
   using EventPtr = std::shared_ptr< Event >;

   class EventRegistry;
   using EventRegistryPtr = std::shared_ptr< EventRegistry >;

   // class IEventConsumer;

} // namespace base
