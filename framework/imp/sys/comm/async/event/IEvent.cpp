#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/application/ThreadIPC.hpp"
#include "api/sys/application/Thread.hpp"
#include "api/sys/application/Process.hpp"
#include "api/sys/application/Context.hpp"
#include "api/sys/comm/async/event/IEvent.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IEvent"


using namespace base::async;



using tRegistry = std::map< tAsyncTypeID, IEvent::tCreator >;
tRegistry s_registry;



bool IEvent::check_in( const tAsyncTypeID& event_type, tCreator p_creator )
{
   if( nullptr == p_creator )
      return false;

   s_registry.emplace( std::make_pair( event_type, p_creator ) );
   return true;
}

void IEvent::dump( )
{
   for( auto& pair : s_registry )
   {
      SYS_VRB( "name: %s / creator: %p", pair.first.c_str( ), pair.second );
   }
}

bool IEvent::serialize( dsi::tByteStream& stream, IEvent::tSptr p_event )
{
   return IEvent::serialize( stream, *p_event );
}

bool IEvent::serialize( dsi::tByteStream& stream, const IEvent& event )
{
   if( s_registry.end( ) == s_registry.find( event.signature( )->type_id( ) ) )
   {
      SYS_ERR( "event '%s' is not registered", event.signature( )->name( ).c_str( ) );
      return false;
   }

   if( false == event.to_stream( stream ) )
   {
      SYS_ERR( "event '%s' serialization error", event.signature( )->name( ).c_str( ) );
      return false;
   }

   return true;
}

IEvent::tSptr IEvent::deserialize( dsi::tByteStream& stream )
{
   tAsyncTypeID event_type;
   if( false == stream.get( event_type ) )
   {
      SYS_ERR( "meta data deserialization error" );
      return nullptr;
   }

   auto iterator = s_registry.find( event_type );
   if( s_registry.end( ) == iterator )
   {
      SYS_ERR( "event '%s' is not registered", event_type.c_str( ) );
      return nullptr;
   }

   IEvent::tSptr p_event = iterator->second( );
   if( false == p_event->from_stream( stream ) )
   {
      SYS_ERR( "event '%s' deserialization error", event_type.c_str( ) );
      return nullptr;
   }

   return p_event;
}



const bool IEvent::set_notification( IAsync::IConsumer* p_consumer, const ISignature::tSptr p_signature )
{
   application::IThread::tSptr p_thread = application::Process::instance( )->current_thread( );
   if( nullptr == p_thread )
   {
      SYS_ERR( "subscribe on event not from application thread" );
      return false;
   }

   SYS_INF( "event: %s / consumer: %p / application thread: %s", p_signature->name( ).c_str( ), p_consumer, p_thread->name( ).c_str( ) );
   p_thread->set_notification( p_signature, p_consumer );

   return true;
}

const bool IEvent::clear_notification( IAsync::IConsumer* p_consumer, const ISignature::tSptr p_signature )
{
   application::IThread::tSptr p_thread = application::Process::instance( )->current_thread( );
   if( nullptr == p_thread )
   {
      SYS_ERR( "unsubscribe from event not from application thread" );
      return false;
   }

   SYS_INF( "event: %s / consumer: %p / application thread: %s", p_signature->name( ).c_str( ), p_consumer, p_thread->name( ).c_str( ) );
   p_thread->clear_notification( p_signature, p_consumer );

   return true;
}

const bool IEvent::clear_all_notifications( IAsync::IConsumer* p_consumer, const ISignature::tSptr p_signature )
{
   application::IThread::tSptr p_thread = application::Process::instance( )->current_thread( );
   if( nullptr == p_thread )
   {
      SYS_ERR( "unsubscribe from event not from application thread" );
      return false;
   }

   SYS_INF( "event: %s / consumer: %p / application thread: %s", p_signature->name( ).c_str( ), p_consumer, p_thread->name( ).c_str( ) );
   p_thread->clear_all_notifications( p_signature, p_consumer );

   return true;
}

const bool IEvent::send( tSptr p_event, const application::Context& to_context )
{
   if( !p_event ) return false;
   if( !( p_event->signature( ) ) ) return false;

   SYS_VRB( "event: %s", p_event->signature( )->name( ).c_str( ) );

   if( to_context.is_external( ) )
   {
      SYS_INF( "sending IPC event" );
      application::IThread::tSptr p_thread_ipc = application::Process::instance( )->thread_ipc( );
      if( nullptr == p_thread_ipc )
      {
         SYS_ERR( "application IPC thread is not started" );
         return false;
      }

      return std::static_pointer_cast< application::ThreadIPC >( p_thread_ipc )->send( p_event, to_context );
   }
   else if( application::Context::thread::broadcast == to_context.tid( ) )
   {
      SYS_INF( "sending broadcast event to all application threads" );
      bool result = true;

      application::IThread::tSptr p_thread_ipc = application::Process::instance( )->thread_ipc( );
      if( nullptr == p_thread_ipc )
      {
         SYS_ERR( "application IPC thread is not started" );
         return false;
      }
      result &= p_thread_ipc->insert_event( p_event );

      application::IThread::tSptrList thread_list = base::application::Process::instance( )->thread_list( );
      for( auto p_thread : thread_list )
         result &= p_thread->insert_event( p_event );

      return result;
   }
   else if( application::Context::thread::local == to_context.tid( ) )
   {
      SYS_INF( "sending event to current application thread" );
      application::IThread::tSptr p_thread = base::application::Process::instance( )->current_thread( );
      if( nullptr == p_thread )
      {
         SYS_ERR( "sending event to unknown application thread" );
         return false;
      }

      return p_thread->insert_event( p_event );
   }
   else
   {
      SYS_INF( "sending event to %s application thread", to_context.tid( ).name( ).c_str( ) );
      application::IThread::tSptr p_thread = base::application::Process::instance( )->thread( to_context.tid( ) );
      if( nullptr == p_thread )
      {
         SYS_ERR( "sending event to unknown application thread" );
         return false;
      }

      return p_thread->insert_event( p_event );
   }

   return true;
}

void IEvent::process( IAsync::IConsumer* p_consumer ) const
{
   if( nullptr == p_consumer )
   {
      SYS_ERR( "consumer: %p", p_consumer );
      return;
   }

   SYS_INF( "consumer: %p", p_consumer );
   process_event( p_consumer );
}
