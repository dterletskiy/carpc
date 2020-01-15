#include "oswrappers/Thread.hpp"
#include "oswrappers/linux/socket/Helpers.hpp"
#include "oswrappers/linux/memory/Hooks.hpp"
extern MemoryMap s_memory_map;

#include "service/ServiceProcess.hpp"
#include "application/components/OnOff.hpp"
#include "application/components/Driver.hpp"
#include "application/components/Master.hpp"
#include "application/components/Slave.hpp"

#include "Trace.hpp"
#define CLASS_ABBR "MAIN"




// void thread_loop( int index )
// {
//    int count = 0;
//    while( count < 10000 )
//    {
//       DBG_TRC( "Thread ID: %#x / index: %d", pthread_self( ), index );
//       ++count;
//    }
// }

// base::os::Thread thread __attribute__ (( section ("THREAD"), init_priority (101) )) = { thread_loop, 45 };

// void __constructor__( ) __attribute__(( constructor(102) ));
// void __destructor__( ) __attribute__(( destructor(102) ));

// void __constructor__( )
// {
//    DBG_INF( );
//    // thread.run( );
// }

// void __destructor__( )
// {
//    DBG_INF( );
//    // thread.join( );
// }











int main( int argc, char *argv[] )
{
   s_memory_map.dump( );

   REGISTER_DSI_EVENT( DsiServiceEvent, DsiService );
   base::EventRegistry::instance( )->dump( );

   base::ServiceInfoVector services =
   {
        { "OnOff_Service", { application::onoff::creator }, 5 }
      , { "Driver_Service", { application::driver::creator }, 10 }
      , { "Device_Service", { application::master::creator, application::slave::creator }, 10 }
   };

   base::IServiceProcessPtr p_process = base::ServiceProcess::instance( );
   if( p_process->start( services ) )
   {
      DBG_MSG( "Booting..." );
      p_process->boot( );
   }

   s_memory_map.dump( );



   DBG_TRC( "Main: program exiting." );
   DBG_INF( "Main: program exiting." );
   DBG_MSG( "Main: program exiting." );
   DBG_WRN( "Main: program exiting." );
   DBG_ERR( "Main: program exiting." );

   return 0;
}
