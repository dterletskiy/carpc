// Framework
#include "api/sys/application/Process.hpp"
#include "api/sys/tools/Tools.hpp"
// Application
#include "imp/hmi/components/onoff/Component.hpp"

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



void boot( int argc, char** argv )
{
   memory::dump( );

   SYS_TRACE_INFORMATION;
   MSG_TRACE_INFORMATION;

   MSG_DBG( "argc = %d", argc );
   MSG_DBG( "SIGRTMIN = %d / SIGRTMAX = %d", SIGRTMIN, SIGRTMAX );

   DUMP_IPC_EVENTS;

   base::application::Thread::Configuration::tVector services =
   {
        { "OnOff_Service", { hmi::components::onoff::Component::creator }, 5 }
   };

   base::application::Process::tSptr p_process = base::application::Process::instance( argc, argv );
   if( p_process->start( services ) )
   {
      MSG_DBG( "Booting..." );
      p_process->boot( );
   }

   memory::dump( );
}

bool test( int argc, char* argv[ ] );



#if OS == OS_LINUX

   void __constructor__( ) __attribute__(( constructor(101) ));
   void __constructor__( )
   {
      MSG_INF( "starting..." );
      base::trace::Logger::init( base::trace::eLogStrategy::CONSOLE, "HMI" );
   }

   void __destructor__( ) __attribute__(( destructor(101) ));
   void __destructor__( )
   {
      MSG_INF( "finishing..." );
   }

   int main( int argc, char* argv[ ] )
   {
      if( test( argc, argv ) )
         boot( argc, argv );

      return 0;
   }

#elif OS == OS_ANDROID

   #include <jni.h>
   #include "api/sys/oswrappers/Thread.hpp"

   void __constructor__( ) __attribute__(( constructor(101) ));
   void __constructor__( )
   {
      base::trace::Logger::init( base::trace::eLogStrategy::ANDROID, "APP" );
      MSG_INF( "library loaded" );
   }

   void __destructor__( ) __attribute__(( destructor(101) ));
   void __destructor__( )
   {
      MSG_INF( "library unloaded" );
   }

   base::os::Thread boot_thread __attribute__ (( section ("THREAD"), init_priority (102) )) = { boot, 1, nullptr };

   extern "C" JNIEXPORT jstring JNICALL
   Java_com_tda_framework_MainActivity_jniStartFramework( JNIEnv* env, jobject /* this */ )
   {
      MSG_VRB( "JNI" );
      boot_thread.run( );

      return env->NewStringUTF( "@TDA: Hello from C++" );
   }

#endif





bool test( int argc, char* argv[ ] )
{
   return true;
}
