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

bool Thread::set_notification( const event::Info& event_info, std::shared_ptr< event::IEventConsumer > p_consumer )
{
   auto iterator = m_subscriptions.find( event_info );
   if( m_subscriptions.end( ) == iterator )
   {
      auto result = m_subscriptions.insert( std::make_pair( event_info, tEventConsumers{ } ) );
      if( !result.second )
         return false;
      iterator = result.first;
   }
   iterator->second.insert( p_consumer );

   return true;
}

bool Thread::clear_notification( const event::Info& event_info, std::shared_ptr< event::IEventConsumer > p_consumer )
{
   auto iterator = m_subscriptions.find( event_info );
   if( m_subscriptions.end( ) == iterator )
      return false;

   iterator->second.erase( p_consumer );
   return true;
}

bool Thread::insert_event( std::shared_ptr< event::IEvent > p_event )
{
   auto lambda = [this, p_event]( )
   {
      const event::Info event_info{ p_event->class_id( ), p_event->id( ) };
      auto iterator = m_subscriptions.find( event_info );
      if( m_subscriptions.end( ) == iterator )
         return;

      for( auto wp_consumer : iterator->second )
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
