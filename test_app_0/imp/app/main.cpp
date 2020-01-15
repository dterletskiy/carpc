// Framework
#include "api/sys/oswrappers/linux/kernel.hpp"
#include "api/sys/service/ServiceProcess.hpp"
// Application
#include "imp/app/components/OnOff.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "MAIN"



namespace memory {
   #ifdef HOOK_MEMORY_ALLOCATOR
      extern void dump( );
      extern void set_track_size( const size_t );
   #else
      inline void dump( ) { }
      inline void set_track_size( const size_t ) { }
   #endif
}

void boot( )
{
   memory::dump( );

   REGISTER_DSI_EVENT( ServiceDSI, PingEvent );
   base::EventRegistry::instance( )->dump( );

   base::ServiceInfoVector services = { { "OnOff_Service", { application::onoff::creator }, 5 } };

   base::ServiceProcessPtr p_process = base::ServiceProcess::instance( );
   if( p_process->start( services ) )
   {
      DBG_MSG( "Booting..." );
      p_process->boot( );
   }



   DBG_TRC( "Main: program exiting." );
   DBG_INF( "Main: program exiting." );
   DBG_MSG( "Main: program exiting." );
   DBG_WRN( "Main: program exiting." );
   DBG_ERR( "Main: program exiting." );

   memory::dump( );
}



template< typename T >
void foo( const std::optional< T >& )
{
   DBG_MSG( "optional" );
}

template< typename T >
void foo( const T& )
{
   DBG_MSG( "not optional" );
}

#include "api/sys/common/ByteBufferT.hpp"

int main( int argc, char *argv[] )
{
   std::optional< int > push_value = std::nullopt;
   DBG_MSG( "push_value = %d", push_value.value_or( 0xFFFFFFFF ) );

   base::ByteBufferT buffer;
   buffer.push( push_value );
   std::optional< int > pop_value = 123;
   buffer.pop( pop_value );
   DBG_MSG( "pop_value = %d", pop_value.value_or( 0xFFFFFFFF ) );







   // boot( );
   return 0;
}



#if 0 // Experiments
#include "oswrappers/Thread.hpp"

void thread_loop( int index )
{
   int count = 0;
   while( count < 10000 )
   {
      DBG_TRC( "Thread ID: %#x / index: %d", pthread_self( ), index );
      ++count;
   }
}

base::os::Thread thread __attribute__ (( section ("THREAD"), init_priority (101) )) = { thread_loop, 45 };

void __constructor__( ) __attribute__(( constructor(102) ));
void __destructor__( ) __attribute__(( destructor(102) ));

void __constructor__( )
{
   DBG_INF( );
   // thread.run( );
}

void __destructor__( )
{
   DBG_INF( );
   // thread.join( );
}
#endif
