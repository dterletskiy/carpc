#pragma once

#include <memory>

#define USE_STREAM



namespace base {

   enum class eCommType : size_t { IPC, ITC, ETC };

   class IEventConsumer;

   class Event;
   using EventPtr = std::shared_ptr< Event >;

   class EventRegistry;
   using EventRegistryPtr = std::shared_ptr< EventRegistry >;

} // namespace base
