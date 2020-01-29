#pragma once

#include "api/sys/comm/event/Types.hpp"



namespace base {

   struct IEventSignature
   {
      IEventSignature( ) = default;
      IEventSignature( const IEventSignature& ) = default;
      virtual ~IEventSignature( ) = default;

      virtual const IEventSignature* const create_copy( ) const = 0;

      virtual bool operator==( const IEventSignature& ) const = 0;
      virtual bool operator<( const IEventSignature& ) const = 0;

      virtual const std::string name( ) const = 0;
      virtual const char* const c_str( ) const = 0;
      virtual const EventTypeID& type_id( ) const = 0;
      virtual const eEventType type( ) const = 0;
   };

} // namespace base
