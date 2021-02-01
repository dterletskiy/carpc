// Framework
#include "api/sys/application/Process.hpp"
#include "api/sys/tools/Tools.hpp"
// Application

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



const bool run( int argc, char** argv );



#if OS == OS_LINUX

   int main( int argc, char* argv[ ] )
   {
      base::trace::Logger::init( base::trace::eLogStrategy::DLT, "EXP" );

      run( argc, argv );

      return 0;
   }

#elif OS == OS_ANDROID

   #include <jni.h>
   #include "api/sys/oswrappers/Thread.hpp"

   base::os::Thread boot_thread __attribute__ (( section ("THREAD"), init_priority (101) )) = { boot, 1, nullptr };

   void __constructor__( ) __attribute__(( constructor(102) ));
   void __destructor__( ) __attribute__(( destructor(102) ));

   void __constructor__( ) { MSG_INF( "library loaded" ); }
   void __destructor__( ) { MSG_INF( "library unloaded" ); }

   extern "C" JNIEXPORT jstring JNICALL
   Java_com_tda_framework_MainActivity_jniStartFramework( JNIEnv* env, jobject /* this */ )
   {
      base::trace::Logger::init( base::trace::eLogStrategy::ANDROID_T, "EXP" );
      MSG_VRB( "JNI" );
      boot_thread.run( );

      return env->NewStringUTF( "@TDA: Hello from C++" );
   }

#endif





















#include "imp/experimental/base/application/Application.hpp"
#include "imp/experimental/components/onoff/Component.hpp"
#include "imp/experimental/components/system/Component.hpp"



void thread_loop( const std::size_t index )
{
   while( true )
      MSG_DBG( "%zu -> 0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF", index );
}

const bool run( int argc, char** argv )
{
   MSG_DBG( "argc = %d", argc );

   // std::vector< base::os::Thread* > threads;
   // for( std::size_t index = 0; index < 100; ++index )
   //    threads.push_back( new base::os::Thread( thread_loop, index ) );

   // for( auto thread : threads )
   //    thread->run( );

   // for( auto thread : threads )
   //    thread->join( );











   SYS_ERR( "--------------- MARKER ---------------" );


   const std::list< fw::application::Thread::Configuration > configuration =
   {
        { "System_Thread", { component::system::Component::creator } },
        { "OnOff_Thread", { component::onoff::Component::creator } },
   };

   auto& application = fw::application::Application::instance( );
   application.run( configuration );



   return false;
}
