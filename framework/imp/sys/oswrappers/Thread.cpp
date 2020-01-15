#if OS == STD
   #include <sstream>
#endif
#if OS == LINUX
   #include <string.h>
#endif
#if OS == WINDOWS
#endif

#include "api/sys/tools/Tools.hpp"
#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/oswrappers/Thread.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OS_THREAD"

namespace base::os {



Thread::Thread( FunctionThread&& p_function )
   : mp_function( p_function )
#if OS == STD
   , m_thread{ }
#endif
{
   SYS_INF( );
   #if OS == LINUX
      pthread_attr_init( &m_attr );
      pthread_attr_setdetachstate( &m_attr, PTHREAD_CREATE_JOINABLE );
      pthread_attr_setscope( &m_attr,PTHREAD_SCOPE_PROCESS );
   #endif
}

Thread::~Thread( )
{
   SYS_INF( );
   if( true == m_created )
   {
      #if OS == STD
         // @TDA: https://habr.com/post/306332/
         if( m_thread.joinable( ) ) m_thread.join( );
      #elif OS == LINUX
         pthread_cancel( m_created );
      #endif
   }
}

const TID Thread::current_id( )
{
   #if OS == STD
      return std::this_thread::get_id( );
   #elif OS == LINUX
      return pthread_self( );
   #endif
}

const std::uint64_t Thread::convert( const TID& tid )
{
   #if OS == STD
      // @TDA: https://stackoverflow.com/questions/7432100/how-to-get-integer-thread-id-in-c11
      std::stringstream ss;
      ss << tid;
      uint64_t uint64_id = std::stoull( ss.str() );
   #elif OS == LINUX
      std::uint64_t uint64_id = 0;//static_cast< std::uint64_t >( id );
      memcpy( &uint64_id, &tid, std::min( sizeof(uint64_id), sizeof(tid) ) );
   #endif

   return uint64_id;
}

#if OS == LINUX
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
#endif

bool Thread::run( )
{
   if( true == m_created )
   {
      SYS_WRN( "Thread: %#lx. Already created", m_id );
      return false;
   }

   #if OS == LINUX
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
   #elif OS == STD
      m_thread = std::thread{ mp_function };
      m_id = m_thread.get_id( );
      SYS_INF( "Thread: %#lx. Created", convert( m_id ) );
      m_created = true;
   #endif

   return m_created;
}

bool Thread::join( )
{
   if( false == m_created ) 
   {
      SYS_WRN( "Thread was not created" );
      return false;
   }

   #if OS == LINUX
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
   #elif OS == STD
      if( true == m_thread.joinable( ) )
      {
         m_thread.join( );
         SYS_INF( "Thread: %#x. Joined", convert( m_id ) );
         m_created = false;
      }
      else
      {
         SYS_WRN( "Thread: %#x. Is not joinable", convert( m_id ) );
      }
   #endif

   return !m_created;
}



} // namespace base::os
