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
