#include "carpc/application/Process.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "MAIN"



namespace memory {

   #ifdef USE_MEMORY_HOOK

      extern void dump( );
      extern void set_track_size( const size_t );
      extern void set_track_file( const char* );

   #else

      inline
      void dump( )
      {
         SYS_WRN( "hooks for mmemory allocators are not activated" );
      }
      inline
      void set_track_size( const size_t )
      {
         SYS_WRN( "hooks for mmemory allocators are not activated" );
      }
      inline
      void set_track_file( const char* )
      {
         SYS_WRN( "hooks for mmemory allocators are not activated" );
      }

   #endif

}



extern const carpc::application::Thread::Configuration::tVector services;

bool test( int argc, char** argv, char** envp );



using tStart = void (*)( int, char**, char** );
using tExit = void (*)( void );

void preinit( int argc, char** argv, char** envp )
{
   carpc::tools::PCE configuration( argc, argv, envp );
   // configuration.print( );

   const carpc::trace::eLogStrategy trace_strategy = carpc::trace::log_strategy_from_string(
         configuration.value_or( "trace_log", "CONSOLE" ).c_str( )
      );
   const std::size_t trace_buffer = static_cast< std::size_t >( std::stoll(
         configuration.value_or( "trace_buffer", "4096" )
      ) );
   const std::string trace_app_name = configuration.value_or( "trace_app_name", "APP" );
   carpc::trace::Logger::init( trace_strategy, trace_app_name, trace_buffer );

   MSG_INF( "preinit_array" );
}
__attribute__(( section(".preinit_array") )) tStart __preinit__ = preinit;

void init( int argc, char** argv, char** envp )
{
   MSG_INF( "init_array" );
}
__attribute__(( section(".init_array") )) tStart __init__ = init;

void fini( )
{
   MSG_INF( "fini_array" );
}
__attribute__(( section(".fini_array") )) tExit __fini__ = fini;

void __constructor__( ) __attribute__(( constructor(101) ));
void __constructor__( )
{
   MSG_INF( "constructor" );
}

void __destructor__( ) __attribute__(( destructor(101) ));
void __destructor__( )
{
   MSG_INF( "destructor" );
}

void boot( int argc, char** argv, char** envp )
{
   memory::dump( );

   SYS_TRACE_INFORMATION;
   MSG_TRACE_INFORMATION;

   carpc::application::Process::tSptr p_process = carpc::application::Process::instance( argc, argv, envp );
   if( p_process->start( services ) )
   {
      MSG_DBG( "Booting..." );
      p_process->boot( );
   }

   memory::dump( );
}

int main( int argc, char** argv, char** envp )
{
   if( test( argc, argv, envp ) )
      boot( argc, argv, envp );

   return 0;
}



#if OS_TARGET == OS_ANDROID

   #include <jni.h>
   #include "carpc/oswrappers/Thread.hpp"

   carpc::os::Thread boot_thread __attribute__ (( section ("THREAD"), init_priority (102) )) = { boot, 1, nullptr, nullptr };

   extern "C"
   JNIEXPORT jstring JNICALL Java_com_tda_carpc_start( JNIEnv* env, jobject obj, jint id )
   {
      MSG_VRB( "starting carpc runtime: %d", id );
      boot_thread.run( );

      return env->NewStringUTF( "@TDA: Hello from C++" );
   }

   extern "C"
   JNIEXPORT jstring JNICALL Java_com_tda_carpc_stop( JNIEnv* env, jobject obj )
   {
      MSG_VRB( "stopping carpc runtime" );

      return env->NewStringUTF( "@TDA: Hello from C++" );
   }

#endif
