// Framework
#include "api/sys/application/Process.hpp"
#include "api/sys/tools/Tools.hpp"
// Application
#include "imp/app/components/OnOff/Component.hpp"
#include "imp/app/components/Driver/Component.hpp"
#include "imp/app/components/Master/Component.hpp"
#include "imp/app/components/Slave/Component.hpp"

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



#ifdef USE_INSTRUMENTAL
   #include "api/sys/asm/cpu.hpp"
   extern "C" {
      void __cyg_profile_func_enter( void* this_fn, void* call_site ) __attribute__(( no_instrument_function ));
      void __cyg_profile_func_exit( void* this_fn, void* call_site ) __attribute__(( no_instrument_function ));

      void __cyg_profile_func_enter( void* this_fn, void* call_site )
      {
         printf( "ENTER: %p, <-- %p: %lld\n", this_fn, call_site, __rdtsc( ) );
      }
      void __cyg_profile_func_exit( void* this_fn, void* call_site )
      {
         printf( "EXIT:  %p, --> %p: %lld\n", this_fn, call_site, __rdtsc( ) );
      }
   }
#endif



const bool test( int argc, char** argv );

void boot( int argc, char** argv )
{
   memory::dump( );
   DBG_MSG( "SIGRTMIN = %d / SIGRTMAX = %d", SIGRTMIN, SIGRTMAX );

   REGISTER_EVENT( base::onoff::ipc::OnOffEvent );
   REGISTER_EVENT( application::events::AppEvent );
   DUMP_IPC_EVENTS;

   base::application::Thread::Info::tVector services =
   {
        { "OnOff_Service", { application::components::onoff::Component::creator }, 5 }
      , { "Driver_Service", { application::components::driver::Component::creator }, 10 }
      , { "Device_Service", { application::components::master::Component::creator, application::components::slave::Component::creator }, 10 }
   };

   base::application::Process::tSptr p_process = base::application::Process::instance( argc, argv );
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



#if OS == LINUX

   int main( int argc, char* argv[ ] )
   {
      DBG_MSG( "argc = %d", argc );

      if( test( argc, argv ) )
         boot( argc, argv );

      return 0;
   }

#elif OS == ANDROID

   #include <jni.h>
   #include "api/sys/oswrappers/Thread.hpp"

   base::os::Thread boot_thread __attribute__ (( section ("THREAD"), init_priority (101) )) = { boot };

   void __constructor__( ) __attribute__(( constructor(102) ));
   void __destructor__( ) __attribute__(( destructor(102) ));

   void __constructor__( ) { DBG_INF( "library loaded" ); }
   void __destructor__( ) { DBG_INF( "library unloaded" ); }

   extern "C" JNIEXPORT jstring JNICALL
   Java_com_tda_framework_MainActivity_jniStartFramework( JNIEnv* env, jobject /* this */ )
   {
      DBG_TRC( "JNI" );
      boot_thread.run( );

      return env->NewStringUTF( "@TDA: Hello from C++" );
   }

#endif





















#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/thread.hpp>
#include <chrono>
#include <thread>
#include <iostream>





// void run_service( const std::size_t id )
// {
//    DBG_INF( "thread id: %zu - enter", id );
//    ioservice_1.run( );
//    DBG_INF( "thread id: %zu - exit", id );
// };

const bool test( int argc, char** argv )
{
   return true;
   SYS_ERR( "--------------- MARKER ---------------" );

   boost::asio::io_service ioservice_1;
   boost::asio::io_service ioservice_2;



   std::shared_ptr< boost::asio::steady_timer > timer_1 = nullptr;
   timer_1 = std::make_shared< boost::asio::steady_timer >( ioservice_1, std::chrono::seconds{ 3 } );
   timer_1->async_wait(
      [ ]( const boost::system::error_code& ec )
      {
         DBG_MSG( "timer 1 enter" );
         DBG_MSG( "timer 1 exit" );
      }
   );



   std::shared_ptr< boost::asio::steady_timer > timer_2 = nullptr;
   timer_2 = std::make_shared< boost::asio::steady_timer >( ioservice_1, std::chrono::seconds{ 5 } );
   timer_2->async_wait(
      [ &ioservice_1 ]( const boost::system::error_code& ec )
      {
         DBG_MSG( "timer 2 enter" );
         ioservice_1.post( [ ]( ){ DBG_ERR( "---------------" ); } );
         DBG_MSG( "timer 2 exit" );
      }
   );



   boost::asio::steady_timer timer_3{ ioservice_1, std::chrono::seconds{ 10 } };
   timer_3.async_wait(
      [ ]( const boost::system::error_code& ec )
      {
         DBG_MSG( "timer 3 enter" );
         DBG_MSG( "timer 3 exit" );
      }
   );



   // DBG_INF( "---------------" );
   // ioservice_1.run( );
   // DBG_INF( "---------------" );
   // ioservice_1.run( );
   // DBG_INF( "---------------" );



   // std::vector< std::shared_ptr< boost::asio::steady_timer > > timer_vector;
   // for( std::size_t count = 0; count < 100; ++count )
   // {
   //    timer_vector.push_back(
   //       std::make_shared< boost::asio::steady_timer >( ioservice_1, std::chrono::seconds{ 3 } )
   //    );
   //    timer_vector.back( )->async_wait(
   //       [ count ]( const boost::system::error_code& ec )
   //       {
   //          DBG_MSG( "timer %zu enter", count );
   //          DBG_MSG( "timer %zu exit", count );
   //       }
   //    );
   // }



   auto thread_loop = [ ]( boost::asio::io_service& service, const std::size_t id )
   {
      DBG_INF( "thread id: %zu - enter", id );
      service.run( );
      DBG_INF( "thread id: %zu - exit", id );
   };

   std::thread thread_1{ thread_loop, std::ref( ioservice_1 ), 1 };
   std::thread thread_2{ thread_loop, std::ref( ioservice_1 ), 2 };



   thread_1.join( );
   thread_2.join( );






   // auto run_service = [ &ioservice_1 ]( const std::size_t id )
   // {
   //    DBG_INF( "thread id: %zu - enter", id );
   //    ioservice_1.run( );
   //    DBG_INF( "thread id: %zu - exit", id );
   // };
   // boost::thread( boost::bind( run_service, 1 ) );
   // boost::thread( boost::bind( run_service, 2 ) );

   // while( true )
   // {
   //    DBG_MSG( );
   //    sleep( 1 );
   // }



   return false;
}
