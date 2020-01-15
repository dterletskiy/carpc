#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/comm/Event.hpp"
#include "api/sys/comm/EventRegistry.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "EventRegistry"



namespace base {



EventRegistryPtr EventRegistry::mp_instance;



EventRegistry::EventRegistry( )
{
}

EventRegistry::~EventRegistry( )
{
}


EventRegistryPtr EventRegistry::instance( )
{
   os::Mutex mutex( true );
   if( nullptr == mp_instance )
      mp_instance.reset( new EventRegistry( ) );

   return mp_instance;
}

bool EventRegistry::register_event( const std::string& event_type, EventCreator p_creator )
{
   if( nullptr == p_creator )
      return false;

   m_registry.emplace( std::make_pair( event_type, p_creator ) );
   return true;
}

EventPtr EventRegistry::create_event( const std::string& event_type ) const
{
   auto iterator = m_registry.find( event_type );
   if( m_registry.end( ) == iterator )
      return nullptr;

   return iterator->second( eCommType::IPC );
}

EventPtr EventRegistry::create_event( const std::string& event_type, ByteBuffer& buffer ) const
{
   auto iterator = m_registry.find( event_type );
   if( m_registry.end( ) == iterator )
      return nullptr;

   EventPtr p_event = iterator->second( eCommType::IPC );
   p_event->data( buffer );
   return p_event;
}

EventPtr EventRegistry::create_event( ByteBuffer& buffer ) const
{
   std::string event_type;
   buffer >> event_type;
   auto iterator = m_registry.find( event_type );
   if( m_registry.end( ) == iterator )
      return nullptr;

   EventPtr p_event = iterator->second( eCommType::IPC );
   p_event->data( buffer );
   return p_event;
}

void EventRegistry::dump( ) const
{
   for( auto& pair : m_registry )
   {
      SYS_TRC( "name: %s / creator: %p", pair.first.c_str( ), pair.second );
   }
}



} // namespace base
