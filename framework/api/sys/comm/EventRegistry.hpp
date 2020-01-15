#pragma once

#include "api/sys/common/Types.hpp"
#include "Types.hpp"



namespace base {



class ByteBuffer;



/****************************************
 *
 * EventRegistry declaration
 *
 ***************************************/
class EventRegistry
{
   using EventCreator = EventPtr(*)( const eCommType );

public:
   ~EventRegistry( );
   static EventRegistryPtr instance( );

private:
   EventRegistry( );
   static EventRegistryPtr mp_instance;

public:
   bool register_event( const std::string&, EventCreator );
   EventPtr create_event( const std::string& ) const;
   EventPtr create_event( const std::string&, ByteBuffer& ) const;
   EventPtr create_event( ByteBuffer& ) const;

   void dump( ) const;

private:
   std::map< std::string, EventCreator > m_registry;
};



} // namespace base


