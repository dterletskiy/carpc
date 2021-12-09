#include "api/sys/application/Process.hpp"

#include "api/sys/trace/Trace.hpp"
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



extern const base::application::Thread::Configuration::tVector services;
extern const char* const application_name;

bool test( int argc, char** argv, char** envp );



using tStart = void (*)( int, char**, char** );
using tExit = void (*)( void );

void preinit( int argc, char** argv, char** envp )
{
   base::tools::PCE configuration( argc, argv, envp );
   // configuration.print( );

   std::string string_trace_strategy = configuration.value( "trace" ).value_or( "CONSOLE" );
   base::trace::eLogStrategy trace_strategy;
   if( "CONSOLE" == string_trace_strategy )        trace_strategy = base::trace::eLogStrategy::CONSOLE;
   else if( "DLT" == string_trace_strategy )       trace_strategy = base::trace::eLogStrategy::DLT;
   else if( "ANDROID" == string_trace_strategy )   trace_strategy = base::trace::eLogStrategy::ANDROID;
   else                                            trace_strategy = base::trace::eLogStrategy::CONSOLE;

   std::size_t trace_buffer = static_cast< std::size_t >( std::stoll(
         configuration.value( "trace_buffer" ).value_or( "1024" )
      ) );

   base::trace::Logger::init( trace_strategy, application_name, trace_buffer );

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

   base::application::Process::tSptr p_process = base::application::Process::instance( argc, argv, envp );
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



#if OS == OS_ANDROID

   #include <jni.h>
   #include "api/sys/oswrappers/Thread.hpp"

   base::os::Thread boot_thread __attribute__ (( section ("THREAD"), init_priority (102) )) = { boot, 1, nullptr, nullptr };

   extern "C"
   JNIEXPORT jstring JNICALL Java_com_tda_carpc_start( JNIEnv* env, jobject obj, jint id )
   {
      MSG_VRB( "starting carpc framework: %d", id );
      boot_thread.run( );

      return env->NewStringUTF( "@TDA: Hello from C++" );
   }

   extern "C"
   JNIEXPORT jstring JNICALL Java_com_tda_carpc_stop( JNIEnv* env, jobject obj )
   {
      MSG_VRB( "stopping carpc framework" );

      return env->NewStringUTF( "@TDA: Hello from C++" );
   }

#endif
