#include <pthread.h>
#include "imp/experimental/base/application/Thread.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Thread"



using namespace fw::application;



Thread::Thread( const std::vector< component::Base::tCreator >& component_creators, const std::string& name )
   : m_name( name )
   , m_component_creators( component_creators )
{
   DBG_MSG( "%s: created", m_name.c_str( ) );
}

Thread::~Thread( )
{
   DBG_MSG( "%s: destroyed", m_name.c_str( ) );
}

const std::string& Thread::name( ) const
{
   return m_name;
}

void Thread::run( )
{
   m_thread = std::thread( &Thread::execute, this );
   pthread_setname_np( m_thread.native_handle( ), m_name.c_str( ) );
}

void Thread::wait( )
{
   m_thread.join( );
}

void Thread::stop( )
{
   m_context.stop( );
}

void Thread::post( Context::tFunction function )
{
   m_context.post( function );
}

void Thread::execute( )
{
   DBG_MSG( "%s: enter", m_name.c_str( ) );

   for( auto creator : m_component_creators )
      m_components.emplace_back( creator( ) );

   m_context.run( );

   m_components.clear( );

   DBG_MSG( "%s: exit", m_name.c_str( ) );
}

std::thread::id Thread::id( ) const
{
   return m_thread.get_id( );
}

bool Thread::set_notification( const event::tClassID& class_id, const event::IInfo& event_info, std::shared_ptr< event::IEventConsumer > p_consumer )
{
   auto p_event_info = event_info.duplicate( );

   auto iterator_class_id = m_subscriptions.find( class_id );
   if( m_subscriptions.end( ) == iterator_class_id )
   {
      auto result = m_subscriptions.insert( std::make_pair( class_id, tConsumerInfoMap{ { p_event_info, tConsumers{ p_consumer } } } ) );
      if( !result.second )
         return false;
      return true;
   }

   auto iterator_event_info = iterator_class_id->second.find( p_event_info );
   if( iterator_class_id->second.end( ) == iterator_event_info )
   {
      auto result = iterator_class_id->second.insert( std::make_pair( p_event_info, tConsumers{ p_consumer } ) );
      if( !result.second )
         return false;
      return true;
   }

   iterator_event_info->second.insert( p_consumer );
   return true;
}

bool Thread::clear_notification( const event::tClassID& class_id, const event::IInfo& event_info, std::shared_ptr< event::IEventConsumer > p_consumer )
{
   auto iterator_class_id = m_subscriptions.find( class_id );
   if( m_subscriptions.end( ) == iterator_class_id )
      return false;

   auto iterator_event_info = iterator_class_id->second.find( event_info );
   if( iterator_class_id->second.end( ) == iterator_event_info )
      return false;

   iterator_event_info->second.erase( p_consumer );

   return true;
}

bool Thread::is_subsribed( const event::tClassID& class_id, const event::IInfo& event_info ) const
{
   auto iterator_class_id = m_subscriptions.find( class_id );
   if( m_subscriptions.end( ) == iterator_class_id )
      return false;

   auto iterator_event_info = iterator_class_id->second.find( event_info );
   if( iterator_class_id->second.end( ) == iterator_event_info )
      return false;

   return true;   
}

bool Thread::insert_event( std::shared_ptr< event::IEvent > p_event )
{
   auto lambda = [this, p_event]( )
   {
      auto iterator_class_id = m_subscriptions.find( p_event->class_id( ) );
      if( m_subscriptions.end( ) == iterator_class_id )
         return;

      auto iterator_event_info = iterator_class_id->second.find( p_event->info( ) );
      if( iterator_class_id->second.end( ) == iterator_event_info )
         return;

      for( auto wp_consumer : iterator_event_info->second )
      {
         if( auto sp_consumer = wp_consumer.lock( ) )
         {
            p_event->process( sp_consumer );
         }
      }
   };
   post( lambda );

   return true;
}
