#include "imp/experimental/base/application/Application.hpp"
#include "imp/experimental/base/event/IEvent.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IEvent"



using namespace fw::event;



bool IEvent::set_notification( std::shared_ptr< IEventConsumer > p_consumer, const tClassID& class_id, const IInfo& info )
{
   if( auto p_thread = application::Application::instance( ).current_thread( ) )
      return p_thread->set_notification( class_id, info, p_consumer );
   return false;
}

bool IEvent::clear_notification( std::shared_ptr< IEventConsumer > p_consumer, const tClassID& class_id, const IInfo& info )
{
   if( auto p_thread = application::Application::instance( ).current_thread( ) )
      return p_thread->clear_notification( class_id, info, p_consumer );
   return false;
}

bool IEvent::send( std::shared_ptr< IEvent > p_event )
{
   for( auto p_thread : application::Application::instance( ).threads( ) )
      if( p_thread )
         p_thread->insert_event( p_event );

   return true;
}