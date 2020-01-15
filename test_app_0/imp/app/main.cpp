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



#if 0
extern "C" {
   void __cyg_profile_func_enter( void* this_fn, void* call_site ) __attribute__((no_instrument_function));
   void __cyg_profile_func_exit( void *this_fn, void *call_site ) __attribute__(( no_instrument_function ));

   void __cyg_profile_func_enter( void* this_fn, void* call_site )
   {
      printf( "ENTER: %p, from %p\n", this_fn, call_site );
   }
   void __cyg_profile_func_exit( void *this_fn, void *call_site )
   {
      printf( "EXIT:  %p, from %p\n", this_fn, call_site );
   }
}
#endif



void boot( )
{
   memory::dump( );

   REGISTER_DSI_EVENT( ServiceDSI, PingEvent );
   REGISTER_DSI_EVENT( ServiceOnOff, OnOffEvent );
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



enum class eDummy: size_t { dummy };
DEFINE_ENUM( eEvent, size_t, request, response );
// enum class eEvent: size_t { request, response };

template< typename TYPE >
class Event
{
public:
   template< typename T = TYPE >
      typename std::enable_if_t< !std::is_same_v< T, eDummy >, TYPE >
         id( )
   {
      return m_id;
   }
   template< typename T = TYPE >
      typename std::enable_if_t< !std::is_same_v< T, eDummy >, void >
         print( )
   {
      DBG_MSG( "%s", to_string( m_id ).c_str( ) );
   }

   template< typename T = TYPE >
      static typename std::enable_if_t< !std::is_same_v< T, eDummy >, void >
         print( const TYPE id )
   {
      DBG_MSG( "%s", to_string( id ).c_str( ) );
   }

protected:
   TYPE m_id = static_cast< TYPE >( 0 );
};



int main( int argc, char *argv[] )
{
   DBG_MSG( "argc = %d", argc );
   DBG_MSG( "SIGRTMIN = %d / SIGRTMAX = %d", SIGRTMIN, SIGRTMAX );



#if 0
   Timer timer_1( new ITimerConsumer( 111 ) );
   Timer timer_2( new ITimerConsumer( 222 ) );
   Timer timer_3( new ITimerConsumer( 333 ) );

   timer_1.start( 1 );
   timer_2.start( 3 );
   timer_3.start( 5 );
#endif



   boot( );
   return 0;
}
