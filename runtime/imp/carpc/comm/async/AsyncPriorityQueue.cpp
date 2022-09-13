#include "carpc/comm/async/event/Event.hpp"
#include "carpc/comm/async/AsyncPriorityQueue.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "AsyncPriorityQueue"



using namespace carpc::async;



AsyncPriorityQueue::AsyncPriorityQueue( const tPriority& max_priority, const std::string& name )
   : m_name( name )
{
   SYS_VRB( "'%s': created", m_name.c_str( ) );
   SYS_VRB( "max priority: %u", max_priority.value( ) );

   for( tPriority index = tPriority::zero; index < max_priority; ++index )
      m_collections.push_back( { } );
}

AsyncPriorityQueue::~AsyncPriorityQueue( )
{
   SYS_VRB( "'%s': destroyed", m_name.c_str( ) );
}

bool AsyncPriorityQueue::insert( const IAsync::tSptr p_async )
{
   if( m_freezed.load( ) )
   {
      SYS_VRB( "'%s': async object (%s) with priority %u can't be inserted, because of collection is freezed",
         m_name.c_str( ),
         p_async->signature( )->dbg_name( ).c_str( ),
         p_async->priority( ).value( )
      );
      return false;
   }

   SYS_VRB( "'%s': inserting async object (%s) with priority %u",
         m_name.c_str( ),
         p_async->signature( )->dbg_name( ).c_str( ),
         p_async->priority( ).value( )
      );

   m_buffer_cond_var.lock( );

   // If priority of current async object is higher then max supported priority it will be inserted
   // with highest priority.
   if( p_async->priority( ).value( ) >= m_collections.size( ) )
   {
      m_collections.back( ).push_back( p_async );
   }
   else
   {
      m_collections[ p_async->priority( ) ].push_back( p_async );
   }

   m_buffer_cond_var.notify( );
   m_buffer_cond_var.unlock( );

   return true;
}

IAsync::tSptr AsyncPriorityQueue::get( )
{
   SYS_VRB( "'%s':", m_name.c_str( ) );
   m_buffer_cond_var.lock( );

   // Check if there is any event with any priority starting from max priority to min.
   // Remember maximum priority where event has been found.
   std::optional< tPriority > priority_to_process = std::nullopt;
   for( tPriority index = tPriority( m_collections.size( ) - 1 ); index > tPriority::zero; --index )
   {
      if( false == m_collections[ index ].empty( ) )
      {
         priority_to_process = index;
         break;
      }
   }

   // Waiting for event in case if any event have not been found for any priority.
   // in this case 'priority_to_process == std::nullopt'
   if( std::nullopt == priority_to_process )
   {
      SYS_VRB( "'%s': waiting for async object...", m_name.c_str( ) );
      m_buffer_cond_var.wait( );
   }

   // In case priority is not defined for what event exists collection should be iterated again.
   if( std::nullopt == priority_to_process )
   {
      for( tPriority index = tPriority( m_collections.size( ) - 1 ); index > tPriority::zero; --index )
      {
         if( false == m_collections[ index ].empty( ) )
         {
            priority_to_process = index;
            break;
         }
      }
   }

   auto& collection = m_collections[ priority_to_process.value( ) ];
   IAsync::tSptr p_async = collection.front( );
   collection.pop_front( );
   SYS_VRB( "'%s': received async object (%s) with priority: %u",
         m_name.c_str( ),
         p_async->signature( )->dbg_name( ).c_str( ),
         p_async->priority( ).value( )
      );
   m_buffer_cond_var.unlock( );

   return p_async;
}

void AsyncPriorityQueue::clear( )
{
   m_buffer_cond_var.lock( );
   SYS_INF( "clearing collection..." );
   for( tPriority index = tPriority( m_collections.size( ) - 1 ); index > tPriority::zero; --index )
   {
      m_collections[ index ].clear( );
   }
   m_buffer_cond_var.unlock( );
}

void AsyncPriorityQueue::freeze( )
{
   m_freezed.store( true );
}

void AsyncPriorityQueue::unfreeze( )
{
   m_freezed.store( false );
}

void AsyncPriorityQueue::dump( ) const
{
   SYS_DUMP_START( );
   SYS_INF( "%s:", m_name.c_str( ) );
   for( tPriority index = tPriority( m_collections.size( ) - 1 ); index > tPriority::zero; --index )
   {
      SYS_INF( "   priority: %s", index.dbg_name( ).c_str( ) );
      for( auto p_async : m_collections[ index ] )
      {
         SYS_INF( "      %s", p_async->signature( )->dbg_name( ).c_str( ) );
      }
   }
   SYS_DUMP_END( );
}
