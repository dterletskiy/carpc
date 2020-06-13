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





#include "api/sys/common/CircularBuffer.hpp"
#include "api/sys/common/ByteStream.hpp"
#include "api/sys/common/ByteBufferT.hpp"
#include "api/sys/dsi/Types.hpp"
#include "api/sys/helpers/macros/strings.hpp"

const bool test( int argc, char** argv )
{
   // return true;






   #if 1 // ByteStream test
      using tBuffer = base::ByteStream;

      enum class eEnum { one, two, three };

      tBuffer stream( 64 );

      {
         size_t value_integral = 123;
         eEnum value_enum = eEnum::three;
         std::string value_string = "Hello world!";
         std::optional< std::string > value_optional_string = "OptionalString";
         std::vector< std::string > value_vector_string = { "one", "two", "three", "four", "five" };
         std::map< size_t, std::string > value_map = { { 111, "one" }, { 222, "two" }, { 333, "three" }, { 444, "four" }, { 555, "five" } };
         bool value_bool = true;
         void* value_pointer = &value_integral;

         stream.push( value_integral );
         stream.dump( );
         stream.push( value_enum );
         stream.dump( );
         stream.push( value_string );
         stream.dump( );
         stream.push( value_optional_string );
         stream.dump( );
         stream.push( value_vector_string );
         stream.dump( );
         stream.push( value_map );
         stream.dump( );
         stream.push( value_bool );
         stream.dump( );
         stream.push( value_pointer );
         stream.dump( );



         size_t test_value_integral = 0;
         eEnum test_value_enum = eEnum::one;
         std::string test_value_string = "";
         std::optional< std::string > test_value_optional_string = "";
         std::vector< std::string > test_value_vector_string;
         std::map< size_t, std::string > test_value_map;
         bool test_value_bool = false;
         void* test_value_pointer = nullptr;

         stream.pop( test_value_integral );
         stream.dump( );
         stream.pop( test_value_enum );
         stream.dump( );
         stream.pop( test_value_string );
         stream.dump( );
         stream.pop( test_value_optional_string );
         stream.dump( );
         stream.pop( test_value_vector_string );
         stream.dump( );
         stream.pop( test_value_map );
         stream.dump( );
         stream.pop( test_value_bool );
         stream.dump( );
         stream.pop( test_value_pointer );
         stream.dump( );



         DBG_MSG( "integral result: %s", BOOL_TO_STRING( value_integral == test_value_integral ) );
         DBG_MSG( "enum result: %s", BOOL_TO_STRING( value_enum == test_value_enum ) );
         DBG_MSG( "string result: %s", BOOL_TO_STRING( value_string == test_value_string ) );
         DBG_MSG( "optional result: %s", BOOL_TO_STRING( value_optional_string == test_value_optional_string ) );
         DBG_MSG( "vector result: %s", BOOL_TO_STRING( value_vector_string == test_value_vector_string ) );
         DBG_MSG( "map result: %s", BOOL_TO_STRING( value_map == test_value_map ) );
         DBG_MSG( "bool result: %s", BOOL_TO_STRING( value_bool == test_value_bool ) );
         DBG_MSG( "bool result: %s", BOOL_TO_STRING( value_pointer == test_value_pointer ) );

         DBG_MSG( "%p -> %p", value_pointer, test_value_pointer );
      }
   #endif



   #if 0 // CircularBuffer test
      base::CircularBuffer cb( 10 );
      cb.dump( );

      {
         size_t size = 5;
         uint8_t* buffer = (uint8_t*)malloc( size );
         for( size_t i = 0; i < size; ++i )
            buffer[ i ] = 0xA0 + i;
         base::CircularBuffer::ePush result = cb.push_back( (void*)buffer, size );
         DBG_WRN( "result: %s", base::CircularBuffer::c_str( result ) );
         cb.dump( );
      }

      {
         size_t size = 7;
         uint8_t* buffer = (uint8_t*)malloc( size );
         for( size_t i = 0; i < size; ++i )
            buffer[ i ] = 0xB0 + i;
         base::CircularBuffer::ePush result = cb.push_back( (void*)buffer, size );
         DBG_WRN( "result: %s", base::CircularBuffer::c_str( result ) );
         cb.dump( );
      }

      {
         size_t size = 3;
         uint8_t* buffer = (uint8_t*)malloc( size );
         cb.move_front( (void*)buffer, size );

         for( size_t i = 0; i < size; ++i )
            printf( "%#x ", static_cast< uint8_t* >( buffer )[i] );
         printf( "\n" );

         cb.dump( );
      }

      {
         size_t size = 3;
         uint8_t* buffer = (uint8_t*)malloc( size );
         for( size_t i = 0; i < size; ++i )
            buffer[ i ] = 0xC0 + i;
         base::CircularBuffer::ePush result = cb.push_back( (void*)buffer, size );
         DBG_WRN( "result: %s", base::CircularBuffer::c_str( result ) );
         cb.dump( );
      }

      {
         size_t size = 8;
         uint8_t* buffer = (uint8_t*)malloc( size );
         for( size_t i = 0; i < size; ++i )
            buffer[ i ] = 0xD0 + i;
         base::CircularBuffer::ePush result = cb.push_back( (void*)buffer, size, true );
         DBG_WRN( "result: %s", base::CircularBuffer::c_str( result ) );
         cb.dump( );
      }

      // {
      //    cb.reallocate( 8 );
      //    cb.dump( );
      // }

      {
         const uint8_t buffer[] = { 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xC1 };
         const bool result = cb.cmp( (void*)buffer, 6, 5 );
         DBG_MSG( "result: %s", BOOL_TO_STRING( result ) );
      }
   #endif



   #if 0 // ByteBuffer test
      using tBuffer = base::ByteBufferT;

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
   #endif








   return false;
}
