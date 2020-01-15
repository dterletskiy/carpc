// // Framework
// #include "service/ServiceProcess.hpp"
// // Application
// #include "application/components/OnOff.hpp"
// #include "application/components/Driver.hpp"
// #include "application/components/Master.hpp"
// #include "application/components/Slave.hpp"

// #include "Trace.hpp"
// #define CLASS_ABBR "MAIN"


namespace memory {
   extern void dump( );
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



#define HAVE_STD_OPTIONAL

#ifdef HAVE_STD_OPTIONAL
   #include <optional>

   namespace std::experimental {
      template< typename T >
         using optional = std::optional< T >;
      template< typename T >
         using make_optional = std::make_optional< T >;

      constexpr std::nullopt_t nullopt = std::nullopt;
   }
#else
   #include <experimental/optional>
#endif


// #include <optional>




int main( int argc, char *argv[] )
{
   // std::experimental::optional< bool > value = std::experimental::nullopt;
   // std::optional< bool > value1 = std::nullopt;

   std::experimental::optional< bool > value = std::experimental::make_optional< bool >( true );



#if 0
   memory::dump( );

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



   DBG_TRC( "Main: program exiting." );
   DBG_INF( "Main: program exiting." );
   DBG_MSG( "Main: program exiting." );
   DBG_WRN( "Main: program exiting." );
   DBG_ERR( "Main: program exiting." );

   memory::dump( );
# endif

   return 0;
}
