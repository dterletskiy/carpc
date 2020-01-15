#pragma once

#include "api/sys/comm/Types.hpp"



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
   using EventCreator = EventPtr(*)( const eCommType );
   using Registry = std::map< EventTypeID, EventCreator >;

public:
   ~EventRegistry( );
   static EventRegistryPtr instance( );

private:
   EventRegistry( );
   static EventRegistryPtr mp_instance;

public:
   EventPtr create_event( const EventTypeID& ) const;
   EventPtr create_event( const EventTypeID&, ByteBufferT& ) const;
   EventPtr create_event( ByteBufferT& ) const;

   void dump( ) const;

public:
   bool register_event( const EventTypeID&, EventCreator );
   const Registry& registry( ) const;
private:
   Registry m_registry;
};



inline
const EventRegistry::Registry& EventRegistry::registry( ) const
{
   return m_registry;
}



} // namespace base


