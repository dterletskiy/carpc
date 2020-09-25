#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/service/ServiceIpcThread.hpp"
#include "api/sys/service/ServiceThread.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/comm/event/IEvent.hpp"

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
      SYS_TRC( "name: %s / creator: %p", pair.first.c_str( ), pair.second );
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
      SYS_ERR( "event '%s' is not registered", event.signature( )->name( ).c_str( ) )
      return false;
   }

   if( false == event.to_stream( stream ) )
   {
      SYS_ERR( "event '%s' serialization error", event.signature( )->name( ).c_str( ) )
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
      SYS_ERR( "event '%s' is not registered", event_type.c_str( ) )
      return nullptr;
   }

   IEvent::tSptr p_event = iterator->second( );
   if( false == p_event->from_stream( stream ) )
   {
      SYS_ERR( "event '%s' deserialization error", event_type.c_str( ) )
      return nullptr;
   }

   return p_event;
}



const bool IEvent::set_notification( IAsync::IConsumer* p_consumer, const ISignature& signature )
{
   IServiceThread::tSptr p_service = ServiceProcess::instance( )->current_service( );
   if( nullptr == p_service )
   {
      SYS_ERR( "subscribe on event not from service thread" );
      return false;
   }

   SYS_INF( "event: %s / consumer: %p / service: %s", signature.name( ).c_str( ), p_consumer, p_service->name( ).c_str( ) );
   p_service->set_notification( signature, p_consumer );

   return true;
}

const bool IEvent::clear_notification( IAsync::IConsumer* p_consumer, const ISignature& signature )
{
   IServiceThread::tSptr p_service = ServiceProcess::instance( )->current_service( );
   if( nullptr == p_service )
   {
      SYS_ERR( "unsubscribe from event not from service thread" );
      return false;
   }

   SYS_INF( "event: %s / consumer: %p / service: %s", signature.name( ).c_str( ), p_consumer, p_service->name( ).c_str( ) );
   p_service->clear_notification( signature, p_consumer );

   return true;
}

const bool IEvent::clear_all_notifications( IAsync::IConsumer* p_consumer, const ISignature& signature )
{
   IServiceThread::tSptr p_service = ServiceProcess::instance( )->current_service( );
   if( nullptr == p_service )
   {
      SYS_ERR( "unsubscribe from event not from service thread" );
      return false;
   }

   SYS_INF( "event: %s / consumer: %p / service: %s", signature.name( ).c_str( ), p_consumer, p_service->name( ).c_str( ) );
   p_service->clear_all_notifications( signature, p_consumer );

   return true;
}

const bool IEvent::send( tSptr p_event, const eCommType comm_type )
{
   if( !p_event ) return false;

   const eCommType type = comm_type == eCommType::NONE ? p_event->comm_type( ) : comm_type;
   switch( type )
   {
      case eCommType::ETC:
      {
         IServiceThread::tSptr p_service = ServiceProcess::instance( )->current_service( );
         if( nullptr == p_service )
         {
            SYS_ERR( "sending ETC event not from service thread" );
            return false;
         }

         return p_service->insert_event( p_event );
      }
      case eCommType::ITC:
      {
         bool result = true;
         IServiceThread::tSptrList service_list = base::ServiceProcess::instance( )->service_list( );
         for( auto p_service : service_list )
            result &= p_service->insert_event( p_event );
         return result;
      }
      case eCommType::IPC:
      {
         IServiceThread::tSptr p_service_ipc = ServiceProcess::instance( )->service_ipc( );
         if( nullptr == p_service_ipc )
         {
            SYS_ERR( "ServiceIpcThread is not started" );
            return false;
         }

         return p_service_ipc->insert_event( p_event );
      }
      default: break;
   }

   return true;
}

const bool IEvent::send_to_context( tSptr p_event, IServiceThread::tWptr pw_service )
{
   if( !p_event ) return false;

   IServiceThread::tSptr p_service = pw_service.lock( );
   if( nullptr == p_service )
   {
      SYS_ERR( "sending ETC event to not valid service thread" );
      return false;
   }

   return p_service->insert_event( p_event );
}

void IEvent::process( IAsync::IConsumer* p_consumer ) const
{
   SYS_INF( "consumer: %p", p_consumer );
   if( nullptr != p_consumer ) process_event( p_consumer );
}
