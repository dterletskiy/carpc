// Framework
#include "api/sys/oswrappers/linux/kernel.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/tools/Tools.hpp"
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



#if 0
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






// #include "api/sys/oswrappers/linux/timer.hpp"
// void timer_handler( union sigval sv )
// {
//    timer_t* timer_id = static_cast< timer_t* >( sv.sival_ptr );
//    SYS_INF( "WhatchDog timer: %#lx", (long) *timer_id );
// }

void call_malloc( size_t size )
{
   DBG_MSG( "--------------------------------------" );
   void* p = malloc( size );
   SYS_SIMPLE_TRC( "%p", p );
   free( p );
   DBG_MSG( "--------------------------------------" );
}

void call_realloc( void* address, size_t size )
{
   DBG_MSG( "--------------------------------------" );
   void* p = realloc( address, size );
   SYS_SIMPLE_TRC( "%p", p );
   free( p );
   DBG_MSG( "--------------------------------------" );
}

void call_calloc( size_t number, size_t size )
{
   DBG_MSG( "--------------------------------------" );
   void* p = calloc( number, size );
   SYS_SIMPLE_TRC( "%p", p );
   free( p );
   DBG_MSG( "--------------------------------------" );
}

int main( int argc, char *argv[] )
{
   base::os::linux::back_trace( 0 );
   memory::dump( );

   call_malloc( 123 );
   call_malloc( 456 );
   call_malloc( 123456789 );

#if 0
   memory::dump( );

   REGISTER_DSI_EVENT( DsiPingEvent, DsiService );
   REGISTER_DSI_EVENT( xDsiPingEvent, xDsiService );
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
#endif



   memory::dump( );
   return 0;
}
