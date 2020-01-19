#pragma once

#include "api/sys/common/ByteBufferT.hpp"
#include "api/sys/service/Types.hpp"
#include "api/sys/comm/event/Types.hpp"
#include "api/sys/comm/event/EventConsumer.hpp"



namespace base {



class IEvent
{
public:
   IEvent( ) = default;
   virtual ~IEvent( ) = default;

public:
   static const bool set_notification( bool, IEventConsumer*, const EventTypeID&, const OptEventInfoID );
   static const bool clear_all_notifications( IEventConsumer*, const EventTypeID& );
   static const bool send( EventPtr, const eCommType comm_type = eCommType::NONE );
   static const bool send_to_context( EventPtr, ServicePtrW  );

public:
   virtual const bool is_dsi( ) const = 0;
   virtual const OptEventInfoID is_broadcast( ) const = 0;
   virtual const bool send( const eCommType comm_type = eCommType::NONE ) = 0;
   virtual const bool send_to_context( ServicePtrW ) = 0;
   virtual void process( IEventConsumer* ) = 0;

public:
   // serrialization / deserrialization methods should
   // serrialize / deserrialize all data exapt of type_id and communication type
   virtual const bool to_buffer( ByteBufferT& ) const = 0;
   virtual const bool from_buffer( ByteBufferT& ) = 0;

public:
   virtual const std::string name( ) const = 0;
   virtual const EventTypeID& type_id( ) const = 0;
   virtual const eCommType& comm_type( ) const = 0;
};



} // namespace base
