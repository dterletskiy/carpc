// Framework
#include "api/sys/common/Helpers.hpp"
#include "api/sys/oswrappers/linux/kernel.hpp"
#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/tools/Performance.hpp"
#include "api/sys/tools/Tools.hpp"
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





template< typename T >
void doSomething( T arg )
{
   std::cout << __PRETTY_FUNCTION__ << ": -> " << arg << std::endl;
}

template< typename... Args >
void doSomethingForAll( Args const&... args )
{
   (void)std::initializer_list<int>{ ((void)doSomething(args),0)... };
}



int main( int argc, char *argv[] )
{
#if 1
   memory::dump( );

   REGISTER_DSI_EVENT( ServiceDSI, PingEventDSI );
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



   return 0;
}
