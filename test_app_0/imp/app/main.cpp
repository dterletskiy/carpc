// Framework
#include "api/sys/asm/cpu.hpp"
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




#if 0
extern "C" {
   void __cyg_profile_func_enter( void* this_fn, void* call_site ) __attribute__(( no_instrument_function ));
   void __cyg_profile_func_exit( void *this_fn, void *call_site ) __attribute__(( no_instrument_function ));

   void __cyg_profile_func_enter( void* this_fn, void* call_site )
   {
      printf( "ENTER: %p, <-- %p: %lld\n", this_fn, call_site, __rdtsc( ) );
   }
   void __cyg_profile_func_exit( void *this_fn, void *call_site )
   {
      printf( "EXIT:  %p, --> %p: %lld\n", this_fn, call_site, __rdtsc( ) );
   }
}
#endif



void boot( )
{
   memory::dump( );

   REGISTER_IPC_EVENT( application::events::IPC::PingEvent );
   REGISTER_IPC_EVENT( api::onoff::ipc::OnOffEvent );
   base::EventRegistry::instance( )->dump( );

   base::ServiceInfoVector services = { { "OnOff_Service", { application::onoff::creator }, 5 } };

   base::ServiceProcessPtr p_process = base::ServiceProcess::instance( );
   if( p_process->start( services ) )
   {
      DBG_MSG( "Booting..." );
      sleep(1);
      p_process->boot( );
   }



   DBG_TRC( "Main: program exiting." );
   DBG_INF( "Main: program exiting." );
   DBG_MSG( "Main: program exiting." );
   DBG_WRN( "Main: program exiting." );
   DBG_ERR( "Main: program exiting." );

   memory::dump( );
}



int main( int argc, char *argv[] )
{
   DBG_MSG( "argc = %d", argc );
   DBG_MSG( "SIGRTMIN = %d / SIGRTMAX = %d", SIGRTMIN, SIGRTMAX );

  boot( );
   return 0;
}
