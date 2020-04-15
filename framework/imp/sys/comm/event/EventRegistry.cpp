#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/comm/event/Event.hpp"
#include "api/sys/comm/event/EventRegistry.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "EventRegistry"



namespace base {



EventRegistry::tSptr EventRegistry::mp_instance;

namespace { os::Mutex s_mutex; }
EventRegistry::tSptr EventRegistry::instance( )
{
   base::os::MutexAutoLocker locker( s_mutex );
   if( nullptr == mp_instance )
      mp_instance.reset( new EventRegistry( ) );

   return mp_instance;
}

bool EventRegistry::register_event( const tAsyncTypeID& event_type, EventCreator p_creator )
{
   if( nullptr == p_creator )
      return false;

   m_registry.emplace( std::make_pair( event_type, p_creator ) );
   return true;
}

IEvent::tSptr EventRegistry::create_event( ByteBufferT& buffer ) const
{
   tAsyncTypeID event_type;
   if( false == buffer.pop( event_type ) )
   {
      SYS_ERR( "meta data deserrialization error" );
      return nullptr;
   }

   auto iterator = m_registry.find( event_type );
   if( m_registry.end( ) == iterator )
   {
      SYS_ERR( "event '%s' is not registered", event_type.c_str( ) )
      return nullptr;
   }

   IEvent::tSptr p_event = iterator->second( );
   if( false == p_event->from_buffer( buffer ) )
   {
      SYS_ERR( "event '%s' deserrialization error", event_type.c_str( ) )
      return nullptr;
   }

   return p_event;
}

bool EventRegistry::create_buffer( ByteBufferT& buffer, IEvent::tSptr p_event ) const
{
   if( m_registry.end( ) == m_registry.find( p_event->signature( )->type_id( ) ) )
   {
      SYS_ERR( "event '%s' is not registered", p_event->signature( )->name( ).c_str( ) )
      return false;
   }

   if( false == p_event->to_buffer( buffer ) )
   {
      SYS_ERR( "event '%s' serrialization error", p_event->signature( )->name( ).c_str( ) )
      return false;
   }

   if( false == buffer.push( p_event->signature( )->type_id( ) ) )
   {
      SYS_ERR( "event '%s' meta data serrialization error", p_event->signature( )->name( ).c_str( ) )
      return false;
   }

   return true;
}

void EventRegistry::dump( ) const
{
   for( auto& pair : m_registry )
   {
      SYS_TRC( "name: %s / creator: %p", pair.first.c_str( ), pair.second );
   }
}



} // namespace base
