#pragma once

#include <memory>



namespace base {

   enum class eCommType : size_t { IPC, ITC, ETC, NONE };

   class IEventConsumer;

   class Event;
   using EventPtr = std::shared_ptr< Event >;

   class EventRegistry;
   using EventRegistryPtr = std::shared_ptr< EventRegistry >;

} // namespace base
