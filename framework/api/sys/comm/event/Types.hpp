#pragma once

#include "api/sys/common/Types.hpp"



namespace base {

   using EventTypeID = std::string;
   using ServiceName = std::string;
   using NoServiceType = void;
   using NoIdInfoType = void;

   enum class eCommType : size_t { IPC, ITC, ETC, NONE };
   const char* c_str( const eCommType );

   enum class eEventType : size_t { SIMPLE, SIMPLE_ID, RR, UNDEFINED };
   const char* c_str( const eEventType );

   class IEventSignature;

   class IEventConsumer;

   class IEvent;
   using EventPtr = std::shared_ptr< IEvent >;

   class EventRegistry;
   using EventRegistryPtr = std::shared_ptr< EventRegistry >;

} // namespace base
