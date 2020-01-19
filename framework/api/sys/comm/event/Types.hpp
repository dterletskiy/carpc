#pragma once

#include "api/sys/common/Types.hpp"



namespace base {

   using EventTypeID = std::string;
   using EventInfoID = size_t;
   using OptEventInfoID = std::optional< EventInfoID >;
   using NoServiceType = void;
   using NoIdInfoType = void;

   struct EventSignature
   {
      EventSignature( const EventTypeID&, const OptEventInfoID& );
      EventSignature( const EventSignature& );

      bool operator==( const EventSignature& ) const;
      bool operator!=( const EventSignature& ) const;
      bool operator<( const EventSignature& ) const;
      bool operator>( const EventSignature& ) const;

      EventTypeID    m_type_id;
      OptEventInfoID m_info_id;
   };

   enum class eCommType : size_t { IPC, ITC, ETC, NONE };
   const char* c_str( const eCommType );

   class IEventConsumer;

   class IEvent;
   using EventPtr = std::shared_ptr< IEvent >;

   class EventRegistry;
   using EventRegistryPtr = std::shared_ptr< EventRegistry >;

} // namespace base
