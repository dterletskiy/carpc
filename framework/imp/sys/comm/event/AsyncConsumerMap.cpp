#include "api/sys/comm/event/Event.hpp"
#include "api/sys/comm/event/AsyncConsumerMap.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "EvConsMap"



using namespace base::async;



AsyncConsumerMap::AsyncConsumerMap( const std::string& name )
   : m_name( name )
   , m_processing( name )
{
   SYS_TRC( "'%s': created", m_name.c_str( ) );
}

AsyncConsumerMap::~AsyncConsumerMap( )
{
   SYS_TRC( "'%s': destroyed", m_name.c_str( ) );
}

void AsyncConsumerMap::set_notification( const IAsync::ISignature::tSptr p_signature, IAsync::IConsumer* p_consumer )
{
   if( nullptr == p_consumer ) return;

   SYS_INF( "'%s': async object (%s) / consumer (%p)", m_name.c_str( ), p_signature->name( ).c_str( ), p_consumer );

   const auto& iterator = m_map.find( p_signature );
   if( iterator == m_map.end( ) )
   {
      m_map.emplace(
         std::pair< const IAsync::ISignature::tSptr, tConsumersSet >( p_signature, { p_consumer } )
      );
   }
   else
   {
      iterator->second.emplace( p_consumer );
   }

   if( m_processing.is_processing( p_signature ) )
   {
      m_processing.remove_consumer_to_remove( p_consumer );
   }
}

void AsyncConsumerMap::clear_notification( const IAsync::ISignature::tSptr p_signature, IAsync::IConsumer* p_consumer )
{
   if( nullptr == p_consumer ) return;

   const auto& iterator_map = m_map.find( p_signature );
   if( iterator_map == m_map.end( ) )
      return;

   auto& consumers_set = iterator_map->second;

   if( m_processing.is_processing( p_signature ) )
   {
      m_processing.add_consumer_to_remove( p_consumer );
   }
   else
   {
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
   while( iterator_map != m_map.end( ) )
   {
      if( p_signature->type_id( ) != iterator_map->first->type_id( ) )
      {
         ++iterator_map;
         continue;
      }


      if( m_processing.is_processing( p_signature ) )
      {
         m_processing.add_consumer_to_remove( p_consumer );
         ++iterator_map;
      }
      else
      {
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
            // Set iterator to previous item after removing current one.
            // So we should not increment iterator.
            iterator_map = m_map.erase( iterator_map );
         }
         else
         {
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

const AsyncConsumerMap::tConsumersSet& AsyncConsumerMap::start_process( const IAsync::ISignature::tSptr p_signature )
{
   auto iterator = m_map.find( p_signature );
   if( m_map.end( ) == iterator )
   {
      static const tConsumersSet dummy = { };
      return dummy;
   }

   return m_processing.start( p_signature, iterator->second );
}

bool AsyncConsumerMap::finish_process( )
{
   if( false == m_processing.is_processing( ) )
   {
      SYS_WRN( "'%s': there is no async object in processing", m_name.c_str( ) );
      return false;
   }

   auto iterator = m_map.find( m_processing.signature( ) );
   if( iterator == m_map.end( ) )
   {
      SYS_WRN( "'%s': consumers are not found, but are in processing for async object (%s)",
         m_name.c_str( ), m_processing.signature( )->name( ).c_str( )
      );
      m_processing.reset( );
      return false;
   }

   return m_processing.finish( iterator->second );
}

void AsyncConsumerMap::dump( ) const
{
   SYS_DUMP_START( );
   SYS_INF( "%s:", m_name.c_str( ) )
   for( const auto pair : m_map )
   {
      printf( "%s => ", pair.first->name( ).c_str( ) );
      for( const auto item : pair.second )
         printf( "%p, ", item );
      printf( "\n" );
   }
   SYS_DUMP_END( );
}



AsyncConsumerMap::ProcessingSignature::ProcessingSignature( const std::string& name )
   : m_name( name )
{
}

const AsyncConsumerMap::tConsumersSet& AsyncConsumerMap::ProcessingSignature::start(
      const IAsync::ISignature::tSptr p_signature, tConsumersSet& from_consumers_set
   )
{
   if( mp_signature )
   {
      SYS_WRN( "'%s': consumers are in processing for async object (%s)",
         m_name.c_str( ), mp_signature->name( ).c_str( )
      );
      static const tConsumersSet dummy = { };
      return dummy;
   }

   mp_signature = p_signature;
   // Here we swap local consumers set with original set to process this set while original set
   // is empty since now and could be changed during this processing.
   // When processing will be finished this set (contains origianl set) will be merged with original set
   // what could be already modified.
   m_consumers_to_process.swap( from_consumers_set );
   return m_consumers_to_process;
}

bool AsyncConsumerMap::ProcessingSignature::finish( tConsumersSet& to_consumers_set )
{
   if( nullptr == mp_signature )
   {
      SYS_WRN( "'%s': there is no async object in processing", m_name.c_str( ) );
      return false;
   }

   to_consumers_set.merge( m_consumers_to_process );

   for( const auto& consumer : m_consumers_to_remove )
      to_consumers_set.erase( consumer );

   reset( );
   return true;
}

void AsyncConsumerMap::ProcessingSignature::reset( )
{
   mp_signature = nullptr;
   m_consumers_to_process.clear( );
   m_consumers_to_remove.clear( );
}

bool AsyncConsumerMap::ProcessingSignature::is_processing( ) const
{
   return mp_signature != nullptr;
}

bool AsyncConsumerMap::ProcessingSignature::is_processing( const IAsync::ISignature::tSptr p_signature ) const
{
   if( false == is_processing( ) )
      return false;

   if( *mp_signature < *p_signature )
      return false;
   if( *p_signature < *mp_signature )
      return false;

   return true;
}

const IAsync::ISignature::tSptr AsyncConsumerMap::ProcessingSignature::signature( ) const
{
   return mp_signature;
}

bool AsyncConsumerMap::ProcessingSignature::add_consumer_to_remove( IAsync::IConsumer* p_consumer )
{
   if( nullptr == mp_signature )
   {
      SYS_WRN( "'%s': there is no async object in processing", m_name.c_str( ) );
      return false;
   }

   m_consumers_to_remove.insert( p_consumer );
   return true;
}

bool AsyncConsumerMap::ProcessingSignature::remove_consumer_to_remove( IAsync::IConsumer* p_consumer )
{
   if( nullptr == mp_signature )
   {
      SYS_WRN( "'%s': there is no async object in processing", m_name.c_str( ) );
      return false;
   }

   m_consumers_to_remove.erase( p_consumer );
   return true;
}
