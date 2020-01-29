// Framework
#include "api/sys/service/ServiceProcess.hpp"
// Application
#include "imp/nav/components/OnOff/OnOff.hpp"
#include "imp/nav/components/Driver/Driver.hpp"
#include "imp/nav/components/Master/Master.hpp"
#include "imp/nav/components/Slave/Slave.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "MAIN"



namespace memory {
   #ifdef HOOK_MEMORY_ALLOCATOR
      extern void dump( );
   #else
      inline void dump( ) { }
   #endif
}








void boot( )
{
   base::ServiceInfoVector services =
   {
        { "OnOff_Service", { application::onoff::creator }, 5 }
      , { "Driver_Service", { application::driver::creator }, 10 }
      , { "Device_Service", { application::master::creator, application::slave::creator }, 10 }
   };

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
}

#if OS == LINUX


   int main( int argc, char* argv[] )
   {
      memory::dump( );
      boot( );
      memory::dump( );

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
