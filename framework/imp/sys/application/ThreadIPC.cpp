#include "api/sys/comm/event/Event.hpp"
#include "api/sys/application/Context.hpp"
#include "api/sys/application/ThreadIPC.hpp"
#include "imp/sys/application/SystemEventConsumer.hpp"
#include "imp/sys/application/ServiceEventConsumer.hpp"
#include "api/sys/dsi/Types.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "SrvIPC"



using namespace base::application;



ThreadIPC::ThreadIPC( )
   : IThread( "IPC", 10 )
   , m_thread( std::bind( &ThreadIPC::thread_loop, this ) )
   , m_events( )
   , m_buffer_cond_var( )
   , m_event_consumers_map( )
{
   ;

   SYS_TRC( "'%s': created", m_name.c_str( ) );
}

ThreadIPC::~ThreadIPC( )
{
   SYS_TRC( "'%s': destroyed", m_name.c_str( ) );
}

void ThreadIPC::thread_loop( )
{
   SYS_INF( "'%s': enter", m_name.c_str( ) );
   m_started = true;

   SystemEventConsumer system_event_consumer( *this );
   ServiceEventConsumer service_event_consumer( *this );

   while( started( ) )
   {
      base::async::IAsync::tSptr p_event = get_event( );
      SYS_TRC( "'%s': processing event (%s)", m_name.c_str( ), p_event->signature( )->name( ).c_str( ) );
      notify( p_event );
   }

   SYS_INF( "'%s': exit", m_name.c_str( ) );
}

bool ThreadIPC::start( )
{
   SYS_INF( "'%s': starting", m_name.c_str( ) );
   if( false == m_thread.run( ) )
   {
      SYS_ERR( "'%s': can't be started", m_name.c_str( ) );
      return false;
   }

   if( false == m_send_receive.start( ) )
   {
      SYS_ERR( "'%s': can't be started", m_name.c_str( ) );
      return false;
   }

   return true;
}

void ThreadIPC::stop( )
{
   SYS_INF( "'%s': stopping", m_name.c_str( ) );
   m_started = false;
   m_send_receive.stop( );
}

void ThreadIPC::boot( const std::string& message )
{
   SYS_INF( "'%s': booting", m_name.c_str( ) );
}

void ThreadIPC::shutdown( const std::string& message )
{
   SYS_INF( "'%s': shutting down", m_name.c_str( ) );
   stop( );
}

bool ThreadIPC::insert_event( const base::async::IAsync::tSptr p_event )
{
   if( false == started( ) )
   {
      SYS_WRN( "'%s': is not started", m_name.c_str( ) );
      return false;
   }

   if( false == is_subscribed( p_event ) )
   {
      SYS_INF( "'%s': there are no consumers for event '%s'", m_name.c_str( ), p_event->signature( )->name( ).c_str( ) )
      return false;
   }

   SYS_TRC( "'%s': inserting event (%s)", m_name.c_str( ), p_event->signature( )->name( ).c_str( ) );
   m_buffer_cond_var.lock( );
   m_events.push_back( p_event );
   m_buffer_cond_var.notify( );
   m_buffer_cond_var.unlock( );

   return true;
}

base::async::IAsync::tSptr ThreadIPC::get_event( )
{
   m_buffer_cond_var.lock( );
   if( true == m_events.empty( ) )
   {
      SYS_TRC( "'%s': waiting for event...", m_name.c_str( ) );
      m_buffer_cond_var.wait( );
   }
   base::async::IAsync::tSptr p_event = m_events.front( );
   m_events.pop_front( );
   ++m_processed_events;
   SYS_TRC( "'%s': received event (%s)", m_name.c_str( ), p_event->signature( )->name( ).c_str( ) );
   m_buffer_cond_var.unlock( );

   return p_event;
}

void ThreadIPC::notify( const base::async::IAsync::tSptr p_event )
{
   // Processing runnable IAsync object
   if( base::async::eAsyncType::RUNNABLE == p_event->signature( )->type( ) )
   {
      m_process_started = time( nullptr );
      SYS_TRC( "'%s': start processing runnable at %ld (%s)", m_name.c_str( ), m_process_started.value( ), p_event->signature( )->name( ).c_str( ) );
      p_event->process( );
      SYS_TRC( "'%s': finished processing runnable started at %ld (%s)", m_name.c_str( ), m_process_started.value( ), p_event->signature( )->name( ).c_str( ) );
      m_process_started.reset( );
      return;
   }

   // Processing event IAsync object
   const auto& iterator = m_event_consumers_map.find( p_event->signature( ) );
   if( iterator == m_event_consumers_map.end( ) )
      return;

   // Here we create a copy of consumers set to avoid modifying consumers set during iterating it,
   // for example, during calling "clear_notification" from "process_event".
   auto consumers_set = iterator->second;
   SYS_TRC( "'%s': %zu consumers will be processed", m_name.c_str( ), consumers_set.size( ) );
   for( base::async::IAsync::IConsumer* p_consumer : consumers_set )
   {
      m_process_started = time( nullptr );
      SYS_TRC( "'%s': start processing event at %ld (%s)", m_name.c_str( ), m_process_started.value( ), p_event->signature( )->name( ).c_str( ) );
      p_event->process( p_consumer );
      SYS_TRC( "'%s': finished processing event started at %ld (%s)", m_name.c_str( ), m_process_started.value( ), p_event->signature( )->name( ).c_str( ) );
   }
   m_process_started.reset( );
}

void ThreadIPC::set_notification( const base::async::IAsync::ISignature& signature, base::async::IAsync::IConsumer* p_consumer )
{
   if( nullptr == p_consumer ) return;

   SYS_INF( "'%s': event (%s) / consumer (%p)", m_name.c_str( ), signature.name( ).c_str( ), p_consumer );
   const auto& iterator = m_event_consumers_map.find( &signature );
   if( iterator == m_event_consumers_map.end( ) )
   {
      // Here signature is a temporary object created on a stack in concrete event static function.
      // So we should create a copy of this signature in a heap and store its pointer to consumers map.
      // Later on when number of consumers for this signature becase zero we must delete object by this pointer and remove poinetr from this map.
      auto p_signature = signature.create_copy( );
      m_event_consumers_map.emplace(
         std::pair< const base::async::IAsync::ISignature*, std::set< base::async::IAsync::IConsumer* > >( p_signature, { p_consumer } )
      );
   }
   else
   {
      iterator->second.emplace( p_consumer );
   }
}

void ThreadIPC::clear_notification( const base::async::IAsync::ISignature& signature, base::async::IAsync::IConsumer* p_consumer )
{
   if( nullptr == p_consumer ) return;

   const auto& iterator = m_event_consumers_map.find( &signature );
   if( iterator == m_event_consumers_map.end( ) )
      return;

   iterator->second.erase( p_consumer );

   if( true == iterator->second.empty( ) )
   {
      // Number of events for this event signature is zero (last one has just deleted), so we should remove pointer to this signature from the map
      // and must delete dynamicly created object (in function "set_notification")
      delete iterator->first;
      m_event_consumers_map.erase( iterator );
   }
}

void ThreadIPC::clear_all_notifications( const base::async::IAsync::ISignature& signature, base::async::IAsync::IConsumer* p_consumer )
{
   if( nullptr == p_consumer ) return;

   auto iterator_map = m_event_consumers_map.begin( );
   while( iterator_map != m_event_consumers_map.end( ) )
   {
      if( signature.type_id( ) != iterator_map->first->type_id( ) )
      {
         ++iterator_map;
         continue;
      }

      auto& consumers_set = iterator_map->second;
      auto iterator_set = consumers_set.find( p_consumer );
      if( consumers_set.end( ) == iterator_set )
      {
         ++iterator_map;
         continue;
      }

      consumers_set.erase( iterator_set );
      if( true == consumers_set.empty( ) )
      {
         // Number of events for this event signature is zero (last one has just deleted),
         // so we should remove pointer to this signature from the map
         // and must delete dynamicly created object (in function "set_notification")
         delete iterator_map->first;
         // Set iterator to previous item after removing current one.
         // So we should not increment iterator.
         iterator_map = m_event_consumers_map.erase( iterator_map );
      }
   }
}

bool ThreadIPC::is_subscribed( const base::async::IAsync::tSptr p_event )
{
   if( base::async::eAsyncType::RUNNABLE == p_event->signature( )->type( ) )
      return true;

   return m_event_consumers_map.end( ) != m_event_consumers_map.find( p_event->signature( ) );
}

void ThreadIPC::dump( ) const
{
   SYS_WRN( "------------------------- START DUMP -------------------------" );
   SYS_INF( "%s:", m_name.c_str( ) )
   for( const auto pair : m_event_consumers_map )
   {
      printf( "%s => ", pair.first->name( ).c_str( ) );
      for( const auto item : pair.second )
         printf( "%p, ", item );
      printf( "\n" );
   }
   SYS_WRN( "-------------------------  END DUMP  -------------------------" );
}

bool ThreadIPC::send( const base::async::IAsync::tSptr p_event, const application::Context& to_context )
{
   base::dsi::Packet packet(
      base::dsi::eCommand::BroadcastEvent,
      *(std::static_pointer_cast< base::async::IEvent >( p_event )),
      to_context
   );
   return m_send_receive.send( packet, to_context );
}

bool ThreadIPC::send( const dsi::Packet& packet, const application::Context& to_context )
{
   return m_send_receive.send( packet, to_context );
}
