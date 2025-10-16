#include "carpc/runtime/comm/async/event/Event.hpp"
#include "carpc/runtime/comm/async/AsyncConsumerMap.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "EvConsMap"



using namespace carpc::async;



AsyncConsumerMap::AsyncConsumerMap( const std::string& name )
   : m_name( name )
{
   SYS_VRB( "'%s': created", m_name.c_str( ) );
}

AsyncConsumerMap::~AsyncConsumerMap( )
{
   SYS_VRB( "'%s': destroyed", m_name.c_str( ) );
}

void AsyncConsumerMap::set_notification( const IAsync::ISignature::tSptr p_signature, IAsync::IConsumer* p_consumer )
{
   SYS_INF( "'%s': async object (%s) / consumer (%p)",
         m_name.c_str( ),
         p_signature->dbg_name( ).c_str( ),
         p_consumer
      );

   if( nullptr == p_consumer ) return;

   if( *p_signature == *( mp_processing_async->signature( ) ) )
   {
      m_consumers_to_add.insert( p_consumer );
      m_consumers_to_remove.erase( p_consumer );
   }
   else
   {
      const auto& iterator_map = m_map.find( p_signature );
      if( m_map.end( ) == iterator_map )
      {
         m_map.emplace(
            std::pair< const IAsync::ISignature::tSptr, tConsumersSet >( p_signature, { p_consumer } )
         );
      }
      else
      {
         iterator_map->second.emplace( p_consumer );
      }
   }

}

void AsyncConsumerMap::clear_notification( const IAsync::ISignature::tSptr p_signature, IAsync::IConsumer* p_consumer )
{
   SYS_INF( "'%s': async object (%s) / consumer (%p)",
         m_name.c_str( ),
         p_signature->dbg_name( ).c_str( ),
         p_consumer
      );

   if( nullptr == p_consumer ) return;

   if( *p_signature == *( mp_processing_async->signature( ) ) )
   {
      m_consumers_to_add.erase( p_consumer );
      m_consumers_to_remove.insert( p_consumer );
   }
   else
   {
      const auto& iterator_map = m_map.find( p_signature );
      if( m_map.end( ) == iterator_map )
         return;

      auto& consumers_set = iterator_map->second;
      consumers_set.erase( p_consumer );
      if( true == consumers_set.empty( ) )
      {
         m_map.erase( iterator_map );
      }
   }

}

void AsyncConsumerMap::clear_all_notifications( const IAsync::ISignature::tSptr p_signature, IAsync::IConsumer* p_consumer )
{
   if( nullptr == p_consumer ) return;

   auto iterator_map = m_map.begin( );
   while( m_map.end( ) != iterator_map )
   {
      if( p_signature->type_id( ) != iterator_map->first->type_id( ) )
      {
         ++iterator_map;
         continue;
      }

      if( 0 == consumers_set.erase( p_consumer ) )
      {
         // There was no removed consumer for current 'signature' =>
         // we need to increment 'iterator_map' to check next 'signature'
         ++iterator_map;
      }
      else
      {
         if( true == consumers_set.empty( ) )
         {
            // For current 'signature' consumer has been just removed and there are no
            // other consumers for this 'signature' => we can remove corresponding record
            // for this 'signature' from the map to save the space and indicate that
            // THERE ARE NO SUBSCRIBERS for this 'signature'.
            // In this case we call 'erase' method what removes corresponding record what
            // is pointed by 'iterator_map' and set this iterator to the next one element.
            // This means we do not need to increment 'iterator_map'.
            iterator_map = m_map.erase( iterator_map );
         }
         else
         {
            // There was removed consumer for current 'signature' =>
            // we need to increment 'iterator_map' to check next 'signature'
            ++iterator_map;
         }
      }
   }
}

bool AsyncConsumerMap::is_subscribed( const IAsync::ISignature::tSptr p_signature )
{
   // If any record is presend in DB for current signature this means that there is at least
   // one consumer must be present for this async object.
   // This is the reason why any record for any signature must be deleted in case of
   // there is no any consumers for this signature any more.
   return m_map.end( ) != m_map.find( p_signature );
}

bool AsyncConsumerMap::process( const IAsync::tSptr p_async, std::atomic< time_t >& timestamp )
{
   if( mp_processing_async )
   {
      SYS_WRN( "'%s': can't process async object '%s' because of processing another one '%s'",
            m_name.c_str( ),
            p_async->signature( )->dbg_name( ).c_str( ),
            mp_processing_async->signature( )->dbg_name( ).c_str( )
         );
      return false;
   }


   auto iterator_map = m_map.find( p_async->signature( ) );
   if( m_map.end( ) == iterator_map )
   {
      SYS_WRN( "'%s': consumers are not found for async object (%s)",
            m_name.c_str( ), p_async->signature( )->dbg_name( ).c_str( )
         );
      return false;
   }

   // Here we work with the original consumers collection by referance
   // to avoid copying process.
   // But this could lead to some specific case what is described below.
   // The method for resolving this issue is also described below.
   tConsumersSet& consumers = iterator_map->second;

   mp_processing_async = p_async;

   for( IAsync::IConsumer* p_consumer : consumers )
   {
      timestamp.store( time( nullptr ) );
      SYS_VRB( "'%s': start processing async object at %ld (%s)",
            m_name.c_str( ),
            process_started( ),
            p_async->signature( )->dbg_name( ).c_str( )
         );

      // Here will be called corresponding 'process_event' the corresponding consumer.
      // During this call could be called one of the next methods:
      //    - 'set_notification'
      //    - 'clear_notification'
      //    - 'clear_all_notifications'
      // In this case each of these methods will be called for current context and
      // there will no be a reise condition during accessing 'm_map' resource.
      // There could be only the situation when new consumers will be added or
      // removed existing for the 'signature' what is currently processed.
      // This will lead to breakdown current iteration loop.
      // To avoid this issue was introduced specific logic for subscription and
      // unsubscription for the same 'signature' what is currently under processing. 
      p_async->process( p_consumer );

      SYS_VRB( "'%s': finished processing async object started at %ld (%s)",
            m_name.c_str( ),
            process_started( ),
            p_async->signature( )->dbg_name( ).c_str( )
         );
   }
   timestamp.store( 0 );

   consumers.merge( m_consumers_to_add );
   for( const auto& consumer_to_remove : m_consumers_to_remove )
      consumers.erase( consumer_to_remove );
   m_consumers_to_add.clear( );
   consumer_to_remove.clear( );
   mp_processing_async.reset( );

   return true;
}

void AsyncConsumerMap::dump( ) const
{
   SYS_DUMP_START( );
   SYS_INF( "%s:", m_name.c_str( ) );
   for( const auto& pair : m_map )
   {
      printf( "%s => ", pair.first->dbg_name( ).c_str( ) );
      for( const auto item : pair.second )
         printf( "%p, ", item );
      printf( "\n" );
   }
   SYS_DUMP_END( );
}
