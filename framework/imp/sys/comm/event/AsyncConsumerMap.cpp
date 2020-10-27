#include "api/sys/comm/event/Event.hpp"
#include "api/sys/comm/event/AsyncConsumerMap.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "EvConsMap"



using namespace base::async;



AsyncConsumerMap::AsyncConsumerMap( const std::string& name )
   : m_name( name )
{
   SYS_TRC( "'%s': created", m_name.c_str( ) );
}

AsyncConsumerMap::~AsyncConsumerMap( )
{
   SYS_TRC( "'%s': destroyed", m_name.c_str( ) );
}

void AsyncConsumerMap::set_notification( const IAsync::ISignature& signature, IAsync::IConsumer* p_consumer )
{
   if( nullptr == p_consumer ) return;

   SYS_INF( "'%s': async object (%s) / consumer (%p)", m_name.c_str( ), signature.name( ).c_str( ), p_consumer );
   const auto& iterator = m_map.find( &signature );
   if( iterator == m_map.end( ) )
   {
      // Here signature is a temporary object created on a stack in concrete async object static function.
      // So we should create a copy of this signature in a heap and store its pointer to consumers map.
      // Later on when number of consumers for this signature becase zero we must delete object by this pointer and remove poinetr from this map.
      auto p_signature = signature.create_copy( );
      m_map.emplace(
         std::pair< const IAsync::ISignature*, std::set< IAsync::IConsumer* > >( p_signature, { p_consumer } )
      );
   }
   else
   {
      iterator->second.emplace( p_consumer );
   }
}

void AsyncConsumerMap::clear_notification( const IAsync::ISignature& signature, IAsync::IConsumer* p_consumer )
{
   if( nullptr == p_consumer ) return;

   const auto& iterator = m_map.find( &signature );
   if( iterator == m_map.end( ) )
      return;

   iterator->second.erase( p_consumer );

   if( true == iterator->second.empty( ) )
   {
      // Number of async objectss for this async object signature is zero (last one has just deleted), so we should remove pointer to this signature from the map
      // and must delete dynamicly created object (in function "set_notification")
      delete iterator->first;
      m_map.erase( iterator );
   }
}

void AsyncConsumerMap::clear_all_notifications( const IAsync::ISignature& signature, IAsync::IConsumer* p_consumer )
{
   if( nullptr == p_consumer ) return;

   auto iterator_map = m_map.begin( );
   while( iterator_map != m_map.end( ) )
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
         // Number of async objectss for this async object signature is zero (last one has just deleted),
         // so we should remove pointer to this signature from the map
         // and must delete dynamicly created object (in function "set_notification")
         delete iterator_map->first;
         // Set iterator to previous item after removing current one.
         // So we should not increment iterator.
         iterator_map = m_map.erase( iterator_map );
      }
   }
}

bool AsyncConsumerMap::is_subscribed( const IAsync::ISignature* signature )
{
   return m_map.end( ) != m_map.find( signature );
}

const std::set< IAsync::IConsumer* >& AsyncConsumerMap::consumers( const IAsync::ISignature* signature ) const
{
   const auto& iterator = m_map.find( signature );
   if( iterator == m_map.end( ) )
   {
      static std::set< IAsync::IConsumer* > dummy;
      return dummy;
   }

   return iterator->second;
}

void AsyncConsumerMap::dump( ) const
{
   SYS_WRN( "------------------------- START DUMP -------------------------" );
   SYS_INF( "%s:", m_name.c_str( ) )
   for( const auto pair : m_map )
   {
      printf( "%s => ", pair.first->name( ).c_str( ) );
      for( const auto item : pair.second )
         printf( "%p, ", item );
      printf( "\n" );
   }
   SYS_WRN( "-------------------------  END DUMP  -------------------------" );
}
