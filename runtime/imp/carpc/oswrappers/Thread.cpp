#include <string.h>

#include "carpc/tools/Tools.hpp"
#include "carpc/oswrappers/Mutex.hpp"
#include "carpc/oswrappers/Thread.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "OS_THREAD"



using namespace carpc::os;



namespace {

   using tRegistry = std::map< os_linux::thread::tID, thread::ID >;
   tRegistry s_registry;

   carpc::os::Mutex s_registry_mutex;
   carpc::os::Mutex thread_loop_mutex;

}



Thread::Thread( FunctionThread&& p_function )
   : mp_function( p_function )
{
   SYS_INF( );
   init( );
}

Thread::~Thread( )
{
   SYS_INF( );
   // Not supported by Android
   // if( true == m_created ) pthread_cancel( m_thread_id );
   os::Mutex::AutoLocker locker( s_registry_mutex );
   s_registry.erase( m_thread_id );
}

void Thread::init( )
{
   pthread_attr_init( &m_attr );
   pthread_attr_setdetachstate( &m_attr, PTHREAD_CREATE_JOINABLE );
   pthread_attr_setscope( &m_attr, PTHREAD_SCOPE_PROCESS );
}

const thread::ID Thread::current_id( )
{
   os::Mutex::AutoLocker locker( s_registry_mutex );
   const auto iterator = s_registry.find( pthread_self( ) );
   if( s_registry.end( ) == iterator )
      return thread::ID::invalid;

   return iterator->second;
}

const os_linux::thread::tID Thread::current_thread_id( )
{
    return pthread_self( );
}

void Thread::name( const std::string& _name )
{
   m_name = _name;
   int result = pthread_setname_np( m_thread_id, m_name.c_str( ) );
   if( 0 != result )
   {
      SYS_ERR( "Unable to set thread name. Error: %d", result );
   }
}

void* Thread::thread_loop( void* parameters )
{
   thread_loop_mutex.lock( );
   Thread* p_thread = (Thread*)parameters;

   os_linux::thread::tID id = pthread_self( );
   SYS_INF( "Thread: %#lx. Enter", id );
   if( id != p_thread->m_thread_id )
   {
      SYS_ERR( "Thread ID mistmach: %#lx != %#lx", id, p_thread->m_thread_id );
      thread_loop_mutex.unlock( );
      pthread_exit( nullptr );
   }
   thread_loop_mutex.unlock( );

   SYS_INF( "%lx -> %s", p_thread->m_thread_id, p_thread->m_id.dbg_name( ).c_str( ) );
   s_registry_mutex.lock( );
   s_registry.emplace( std::make_pair( p_thread->m_thread_id, p_thread->m_id ) );
   s_registry_mutex.unlock( );

   p_thread->mp_function( );
   return static_cast< void* >( p_thread );
}

bool Thread::run( const std::string& _name )
{
   if( true == m_created )
   {
      SYS_WRN( "Thread: %#lx. Already created", m_thread_id );
      return false;
   }

   int result = pthread_create( &m_thread_id, &m_attr, thread_loop, this );
   if( 0 == result )
   {
      SYS_INF( "Thread: %#lx. Created", m_thread_id );
      m_created = true;
   }
   else
   {
      SYS_ERR( "Thread was not created. Error: %d", result );
   }

   name( _name );

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
   int result = pthread_join( m_thread_id, status );
   if( 0 == result )
   {
      SYS_INF( "Thread: %#lx. Joined with status: %p", m_thread_id, ((int**)status) );
      m_created = false;
   }
   else
   {
      SYS_ERR( "Thread: %#lx. Was not joined. Error: %d", m_thread_id, result );
   }

   return !m_created;
}
