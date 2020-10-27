#include "api/sys/comm/event/Event.hpp"
#include "api/sys/comm/event/AsyncQueue.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "AsyncQueue"



using namespace base::async;



AsyncQueue::AsyncQueue( const std::string& name )
   : m_name( name )
{
   SYS_TRC( "'%s': created", m_name.c_str( ) );
}

AsyncQueue::~AsyncQueue( )
{
   SYS_TRC( "'%s': destroyed", m_name.c_str( ) );
}

bool AsyncQueue::insert( const IAsync::tSptr p_async )
{
   SYS_TRC( "'%s': inserting async object (%s)", m_name.c_str( ), p_async->signature( )->name( ).c_str( ) );
   m_buffer_cond_var.lock( );
   m_collection.push_back( p_async );
   m_buffer_cond_var.notify( );
   m_buffer_cond_var.unlock( );

   return true;
}

IAsync::tSptr AsyncQueue::get( )
{
   m_buffer_cond_var.lock( );
   if( true == m_collection.empty( ) )
   {
      SYS_TRC( "'%s': waiting for async object...", m_name.c_str( ) );
      m_buffer_cond_var.wait( );
   }
   IAsync::tSptr p_async = m_collection.front( );
   m_collection.pop_front( );
   SYS_TRC( "'%s': received async object (%s)", m_name.c_str( ), p_async->signature( )->name( ).c_str( ) );
   m_buffer_cond_var.unlock( );

   return p_async;
}

void AsyncQueue::dump( ) const
{
   SYS_WRN( "------------------------- START DUMP -------------------------" );
   SYS_INF( "%s:", m_name.c_str( ) )
   for( const auto element : m_collection )
   {
      printf( "%s\n", element->signature( )->name( ).c_str( ) );
   }
   SYS_WRN( "-------------------------  END DUMP  -------------------------" );
}
