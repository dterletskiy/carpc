#include <string.h>

#include "api/sys/tools/Tools.hpp"
#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/oswrappers/Thread.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OS_THREAD"

namespace base::os {



Thread::Thread( FunctionThread&& p_function )
   : mp_function( p_function )
{
   SYS_INF( );
   pthread_attr_init( &m_attr );
   pthread_attr_setdetachstate( &m_attr, PTHREAD_CREATE_JOINABLE );
   pthread_attr_setscope( &m_attr,PTHREAD_SCOPE_PROCESS );
}

Thread::~Thread( )
{
   SYS_INF( );
   // Not supported by Android
   // if( true == m_created ) pthread_cancel( m_created );
}

const TID Thread::current_id( )
{
   return pthread_self( );
}

const std::uint64_t Thread::convert( const TID& tid )
{
   std::uint64_t uint64_id = 0;//static_cast< std::uint64_t >( id );
   memcpy( &uint64_id, &tid, std::min( sizeof(uint64_id), sizeof(tid) ) );

   return uint64_id;
}

Mutex mutex;
void* Thread::thread_loop( void* parameters )
{
   mutex.lock( );
   Thread* p_thread = (Thread*)parameters;

   pthread_t id = pthread_self( );
   SYS_INF( "Thread: %#lx. Enter", id );
   if( id != p_thread->m_id )
   {
      SYS_ERR( "Thread ID mistmach: %#lx != %#lx", id, p_thread->m_id );
      mutex.unlock( );
      pthread_exit( nullptr );
   }
   mutex.unlock( );

   p_thread->mp_function( );
   return static_cast< void* >( p_thread );
}

bool Thread::run( )
{
   if( true == m_created )
   {
      SYS_WRN( "Thread: %#lx. Already created", m_id );
      return false;
   }

   int result = pthread_create( &m_id, &m_attr, thread_loop, this );
   if( 0 == result )
   {
      SYS_INF( "Thread: %#lx. Created", m_id );
      m_created = true;
   }
   else
   {
      SYS_ERR( "Thread was not created. Error: %d", result );
   }

   return m_created;
}

bool Thread::join( )
{
   if( false == m_created ) 
   {
      SYS_WRN( "Thread was not created" );
      return false;
   }

   void** status = nullptr;
   int result = pthread_join( m_id, status );
   if( 0 == result )
   {
      SYS_INF( "Thread: %#lx. Joined with status: %p", m_id, ((int**)status) );
      m_created = false;
   }
   else
   {
      SYS_ERR( "Thread: %#lx. Was not joined. Error: %d", m_id, result );
   }

   return !m_created;
}



} // namespace base::os
