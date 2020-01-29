#pragma once

#include "api/sys/common/ByteBufferT.hpp"
#include "api/sys/helpers/functions/format.hpp"
#include "api/sys/service/Types.hpp"
#include "api/sys/comm/event/Types.hpp"
#include "api/sys/comm/event/EventConsumer.hpp"



namespace base {



class IEvent
{
public:
   IEvent( const eCommType );
   virtual ~IEvent( ) = default;

public:
   static const bool set_notification( IEventConsumer*, const IEventSignature& );
   static const bool clear_notification( IEventConsumer*, const IEventSignature& );
   static const bool clear_all_notifications( IEventConsumer*, const IEventSignature& );
   static const bool send( EventPtr, const eCommType comm_type = eCommType::NONE );
   static const bool send_to_context( EventPtr, ServicePtrW  );

public:
   virtual const bool send( const eCommType comm_type = eCommType::NONE ) = 0;
   virtual const bool send_to_context( ServicePtrW ) = 0;
   virtual void process( IEventConsumer* ) = 0;

public:
   // serrialization / deserrialization methods should
   // serrialize / deserrialize all data exapt of type_id and communication type
   virtual const bool to_buffer( ByteBufferT& ) const = 0;
   virtual const bool from_buffer( ByteBufferT& ) = 0;

public:
   virtual const bool is_ipc( ) const = 0;

public:
   virtual const IEventSignature* const signature( ) const = 0;

public:
   const eCommType& comm_type( );
private:
   eCommType m_comm_type;
};



inline
IEvent::IEvent( const eCommType comm_type )
   : m_comm_type( comm_type )
{
}

inline
const eCommType& IEvent::comm_type( )
{
   if( eCommType::NONE == m_comm_type )
      m_comm_type = is_ipc( ) ? eCommType::IPC : eCommType::ITC;

   return m_comm_type;
}



} // namespace base



namespace base {

   template< typename TYPE >
   inline EventPtr create_event( )
   {
      return TYPE::create( eCommType::IPC );
   }

} // namespace base
