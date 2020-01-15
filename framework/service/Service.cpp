#include "comm/Event.hpp"
#include "ServiceBrocker.hpp"
#include "Service.hpp"

#include "Trace.hpp"
#define CLASS_ABBR "Srv"



namespace base {




Service::Service( IServiceBrockerPtr p_service_brocker, const ServiceInfo& info )
   : mp_service_brocker( p_service_brocker )
   , m_events( )
   , m_name( info.m_name )
   , m_wd_timeout( info.m_wd_timeout )
   , m_component_creators( info.m_component_creators )
{
   mp_thread = std::make_shared< base::os::Thread >( std::bind( &Service::thread_loop, this ) );

   SYS_INF( "'%s': creating", m_name.c_str( ) );
}

Service::~Service( )
{
   SYS_INF( "'%s': destroying", m_name.c_str( ) );
}

const TID Service::id( ) const
{
   TID id;
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

   ServiceEvent::set_notification( true, this );
   DsiService::DsiServiceEvent::set_notification( true, this );

   while( started( ) )
   {
      EventPtr p_event = get_event( );
      SYS_INF( "'%s': processing event (%s)", m_name.c_str( ), p_event->type_id( ).c_str( ) );
      notify( p_event );
   }

   // ServiceEvent::set_notification( false, this );

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
   // clear_all_notifications( );
   m_started = false;
}

bool Service::send_event( const EventPtr p_event )
{
   if( false == m_started )
   {
      SYS_WRN( "'%s': is not started", m_name.c_str( ) );
      return false;
   }

   if( IServiceBrockerPtr p_service_brocker = mp_service_brocker.lock() )
   {
      p_service_brocker->insert_event( p_event );
      return true;
   }
   else
   {
      SYS_ERR( "ServiceBrocker does not exist" );
      return false;
   }
}

bool Service::insert_event( const EventPtr p_event )
{
   if( false == m_started )
   {
      SYS_WRN( "'%s': is not started", m_name.c_str( ) );
      return false;
   }
   SYS_INF( "'%s': inserting event (%s)", m_name.c_str( ), p_event->type_id( ).c_str( ) );

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
      SYS_INF( "'%s': waiting for event...", m_name.c_str( ) );
      m_buffer_cond_var.wait( );
   }
   EventPtr p_event = m_events.front( );
   m_events.pop_front( );
   ++m_processed_events;
   SYS_INF( "'%s': received event (%s)", m_name.c_str( ), p_event->type_id( ).c_str( ) );
   m_buffer_cond_var.unlock( );

   return p_event;
}

void Service::notify( const EventPtr p_event )
{
   const auto& iterator = m_event_consumers_map.find( p_event->type_id( ) );
   if( iterator == m_event_consumers_map.end( ) )
      return;

   SYS_INF( "'%s': %d consumers will be processed", m_name.c_str( ), iterator->second.size( ) );
   for( IEventConsumer* p_consumer : iterator->second )
   {
      m_process_started = time( nullptr );
      SYS_INF( "'%s': start processing at %ld", m_name.c_str( ), m_process_started.value( ) );
      p_event->process( p_consumer );
      SYS_INF( "'%s': finished processing started at %ld", m_name.c_str( ), m_process_started.value( ) );
   }
   m_process_started.reset( );
}

void Service::set_notification( const Event_ID& event_id, IEventConsumer* p_consumer )
{
   if( nullptr == p_consumer ) return;

   SYS_INF( "'%s': event id (%s) / consumer (%p)", m_name.c_str( ), event_id.c_str( ), p_consumer );
   const auto& iterator = m_event_consumers_map.find( event_id );
   if( iterator == m_event_consumers_map.end( ) )
   {
      m_event_consumers_map.emplace(
            std::pair< Event_ID, std::set< IEventConsumer* > >( event_id, { p_consumer } )
         );
   }
   else
   {
      iterator->second.emplace( p_consumer );
   }
}

void Service::clear_notification( const Event_ID& event_id, IEventConsumer* p_consumer )
{
   if( nullptr == p_consumer ) return;

   const auto& iterator = m_event_consumers_map.find( event_id );
   if( iterator == m_event_consumers_map.end( ) )
      return;

   iterator->second.erase( p_consumer );

   if( true == iterator->second.empty( ) )
      m_event_consumers_map.erase( event_id );
}

void Service::clear_all_notifications( )
{
   m_event_consumers_map.clear( );
}

bool Service::is_subscribed( const Event_ID& event_id )
{
   const auto& iterator = m_event_consumers_map.find( event_id );
   if( iterator == m_event_consumers_map.end( ) )
      return false;

   return true;
}

void Service::process_event( const ServiceEvent& event )
{
   SYS_INF( "'%s': command = %#x, info = %s", m_name.c_str( ), event.data( )->command, event.data( )->info.c_str( ) );
   switch( event.data( )->command )
   {
      case eServiceCommand::boot:
      {
         SysEvent::send_event( { eSysCommand::boot, "request boot" }, eCommType::ETC );
         break;
      }
      case eServiceCommand::shutdown:
      {
         stop( );
         break;
      }
      case eServiceCommand::ping:
      {
         break;
      }
      default:
      {
         break;
      }
   }
}

void Service::process_event( const DsiService::DsiServiceEvent& event )
{
   SYS_INF( "'%s': command = %#x, info = %s", m_name.c_str( ), event.data( )->command, event.data( )->info.c_str( ) );
   switch( event.data( )->command )
   {
      case eServiceCommand::boot:
      {
         SysEvent::send_event( { eSysCommand::boot, "request boot" }, eCommType::ETC );
         break;
      }
      case eServiceCommand::shutdown:
      {
         stop( );
         break;
      }
      case eServiceCommand::ping:
      {
         break;
      }
      default:
      {
         break;
      }
   }
}



} // namespace base
