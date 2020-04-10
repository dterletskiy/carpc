#pragma once

#include "api/sys/comm/event/IEvent.hpp"



namespace base {



class ByteBufferT;



/****************************************
 *
 * EventRegistry declaration
 *
 ***************************************/
class EventRegistry
{
public:
   using tSptr = std::shared_ptr< EventRegistry >;
   using EventCreator = IEvent::tSptr(*)( );
   using Registry = std::map< EventTypeID, EventCreator >;

public:
   ~EventRegistry( ) = default;
   static tSptr instance( );

private:
   EventRegistry( ) = default;
   static tSptr mp_instance;

public:
   IEvent::tSptr create_event( ByteBufferT& ) const;
   bool create_buffer( ByteBufferT&, IEvent::tSptr ) const;

public:
   bool register_event( const EventTypeID&, EventCreator );
   const Registry& registry( ) const;
   void dump( ) const;
private:
   Registry m_registry;
};



inline
const EventRegistry::Registry& EventRegistry::registry( ) const
{
   return m_registry;
}



} // namespace base


