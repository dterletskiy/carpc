#include "api/sys/comm/Event.hpp"
#include "api/sys/service/Service.hpp"
#include "imp/sys/service/ServiceEventConsumer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Srv"



namespace base {




Service::Service( const ServiceInfo& info )
   : m_name( info.m_name )
   , m_wd_timeout( info.m_wd_timeout )
   , m_events( )
   , m_buffer_cond_var( )
   , m_event_consumers_map( )
   , m_components( )
   , m_component_creators( info.m_component_creators )
{
   mp_thread = std::make_shared< base::os::Thread >( std::bind( &Service::thread_loop, this ) );

   SYS_TRC( "'%s': created", m_name.c_str( ) );
}

Service::~Service( )
{
   SYS_TRC( "'%s': destroyed", m_name.c_str( ) );
}

const TID Service::id( ) const
{
   TID id = 0;
   if( mp_thread && m_started )
      id = mp_thread->id( );

   return id;
}

void Service::thread_loop( )
{
   SYS_INF( "'%s': enter", m_name.c_str( ) );
   m_started = true;

   // Creating components
   for( auto creator : m_component_creators )
      m_components.emplace_back( creator( shared_from_this( ) ) );

   ServiceEventConsumer consumer( shared_from_this( ) );

   while( started( ) )
   {
      EventPtr p_event = get_event( );
      SYS_TRC( "'%s': processing event (%s)", m_name.c_str( ), p_event->name( ).c_str( ) );
      notify( p_event );
   }

   // Destroying components
   m_components.clear( );

   SYS_INF( "'%s': exit", m_name.c_str( ) );
}

bool Service::start( )
{
   SYS_INF( "Starting service '%s'", m_name.c_str( ) );
   bool result = mp_thread->run( );
   if( false == result )
   {
      SYS_ERR( "Service '%s' can't be started", m_name.c_str( ) );
   }

   return result;
}

void Service::stop( )
{
   SYS_INF( "'%s': stopping", m_name.c_str( ) );
   m_started = false;
}

bool Service::insert_event( const EventPtr p_event )
{
   if( false == started( ) )
   {
      SYS_WRN( "'%s': is not started", m_name.c_str( ) );
      return false;
   }

   SYS_TRC( "'%s': inserting event (%s)", m_name.c_str( ), p_event->name( ).c_str( ) );
   m_buffer_cond_var.lock( );
   m_events.push_back( p_event );
   m_buffer_cond_var.notify( );
   m_buffer_cond_var.unlock( );

   return true;
}

EventPtr Service::get_event( )
{
   m_buffer_cond_var.lock( );
   if( true == m_events.empty( ) )
   {
      SYS_TRC( "'%s': waiting for event...", m_name.c_str( ) );
      m_buffer_cond_var.wait( );
   }
   EventPtr p_event = m_events.front( );
   m_events.pop_front( );
   ++m_processed_events;
   SYS_TRC( "'%s': received event (%s)", m_name.c_str( ), p_event->name( ).c_str( ) );
   m_buffer_cond_var.unlock( );

   return p_event;
}

void Service::notify( const EventPtr p_event )
{
   const auto& iterator = m_event_consumers_map.find( std::make_pair( p_event->type_id( ), p_event->is_broadcast( ) ) );
   if( iterator == m_event_consumers_map.end( ) )
      return;

   auto consumers_set = iterator->second; // @TDA: this copy is needed to avoid modifying consumers set during iterating it.
   SYS_TRC( "'%s': %zu consumers will be processed", m_name.c_str( ), consumers_set.size( ) );
   for( IEventConsumer* p_consumer : consumers_set )
   {
      m_process_started = time( nullptr );
      SYS_TRC( "'%s': start processing at %ld (%s)", m_name.c_str( ), m_process_started.value( ), p_event->name( ).c_str( ) );
      p_event->process( p_consumer );
      SYS_TRC( "'%s': finished processing started at %ld (%s)", m_name.c_str( ), m_process_started.value( ), p_event->name( ).c_str( ) );
   }
   m_process_started.reset( );
}

void Service::set_notification( const EventTypeID& event_type_id, const OptEventInfoID event_info_id, IEventConsumer* p_consumer )
{
   if( nullptr == p_consumer ) return;

   SYS_INF( "'%s': event id (%s) / consumer (%p)", m_name.c_str( ), event_type_id.c_str( ), p_consumer );
   const auto& iterator = m_event_consumers_map.find( std::make_pair( event_type_id, event_info_id ) );
   if( iterator == m_event_consumers_map.end( ) )
   {
      m_event_consumers_map.emplace(
            std::pair< std::pair< EventTypeID, OptEventInfoID >, std::set< IEventConsumer* > >
               ( std::make_pair( event_type_id, event_info_id ), { p_consumer } )
         );
   }
   else
   {
      iterator->second.emplace( p_consumer );
   }
}

void Service::clear_notification( const EventTypeID& event_type_id, const OptEventInfoID event_info_id, IEventConsumer* p_consumer )
{
   if( nullptr == p_consumer ) return;

   const auto& iterator = m_event_consumers_map.find( std::make_pair( event_type_id, event_info_id ) );
   if( iterator == m_event_consumers_map.end( ) )
      return;

   iterator->second.erase( p_consumer );

   if( true == iterator->second.empty( ) )
      m_event_consumers_map.erase( std::make_pair( event_type_id, event_info_id ) );
}

void Service::clear_all_notifications( const EventTypeID& event_type_id, IEventConsumer* p_consumer )
{
   if( nullptr == p_consumer ) return;

   for( auto pair: m_event_consumers_map )
   {
      if( event_type_id != pair.first.first )
         continue;

      auto& consumers_set = pair.second;
      auto iterator = consumers_set.find( p_consumer );
      if( consumers_set.end( ) == iterator )
         continue;

      consumers_set.erase( iterator );
   }
}

bool Service::is_subscribed( const EventPtr p_event )
{
   const auto& iterator = m_event_consumers_map.find( std::make_pair( p_event->type_id( ), p_event->is_broadcast( ) ) );
   if( iterator == m_event_consumers_map.end( ) )
   {
      SYS_TRC( "'%s': is not subscribed on event (%s)", m_name.c_str( ), p_event->name( ).c_str( ) );
      return false;
   }

   return true;
}



} // namespace base
