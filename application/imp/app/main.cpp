// Framework
#include "api/sys/service/ServiceProcess.hpp"
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

   base::tools::cmd::init( argc, argv );

   REGISTER_EVENT( api::onoff::ipc::OnOffEvent );
   base::EventRegistry::instance( )->dump( );

   base::ServiceThread::Info::tVector services =
   {
        { "OnOff_Service", { application::components::onoff::Component::creator }, 5 }
      , { "Driver_Service", { application::components::driver::Component::creator }, 10 }
      , { "Device_Service", { application::components::master::Component::creator, application::components::slave::Component::creator }, 10 }
   };

   base::ServiceProcess::tSptr p_process = base::ServiceProcess::instance( );
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






#include "api/sys/common/ByteBufferT.hpp"
#include "api/sys/dsi/Types.hpp"
   #include "api/sys/helpers/macros/strings.hpp"
using tBuffer = base::ByteBufferT;

const bool test( int argc, char** argv )
{
   return true;


   base::ByteBufferT buffer;
   base::dsi::Packet packet;
   packet.add_package( base::dsi::eCommand::RegisterServer, std::string( "service_one" ) );
   packet.add_package( base::dsi::eCommand::RegisterServer, std::string( "service_two" ) );
   packet.add_package( base::dsi::eCommand::RegisterServer, std::string( "service_two" ) );
   buffer.push( packet );
   buffer.dump( );
   DBG_MSG( "%zu", buffer.size( ) );

   packet = { };
   bool result = packet.from_buffer( buffer );
   DBG_MSG( "%s", BOOL_TO_STRING( result ) );
   for( const base::dsi::Package& package : packet.packages( ) )
   {
      std::string service_name = "xxxxx";
      base::dsi::eCommand command = base::dsi::eCommand::Undefined;
      tBuffer data( package.data( ) );
      data.pop( service_name );
      DBG_MSG( "%s", service_name.c_str( ) );
   }





   {
      const char* string1 = "Hello ";
      const char* string2 = "world!!!";
      tBuffer buffer;
      buffer.push( (const void*)string1, strlen( string1 ) );
      buffer.push( (const void*)string2, strlen( string2 ) );
   }



   return false;
}
