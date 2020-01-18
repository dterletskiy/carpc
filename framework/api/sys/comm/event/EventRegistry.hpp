#pragma once

#include "api/sys/comm/event/Types.hpp"



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
   using EventCreator = EventPtr(*)( );
   using Registry = std::map< EventTypeID, EventCreator >;

public:
   ~EventRegistry( ) = default;
   static EventRegistryPtr instance( );

private:
   EventRegistry( ) = default;
   static EventRegistryPtr mp_instance;

public:
   EventPtr create_event( ByteBufferT& ) const;
   bool create_buffer( ByteBufferT&, EventPtr ) const;

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


