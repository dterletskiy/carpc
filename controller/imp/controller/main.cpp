// Framework
#include "api/sys/application/Process.hpp"
#include "api/sys/tools/Tools.hpp"
// Application
#include "imp/controller/components/onoff/Component.hpp"

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
   DBG_MSG( "SIGRTMIN = %d / SIGRTMAX = %d", SIGRTMIN, SIGRTMAX );

   // REGISTER_EVENT( base::onoff::ipc::OnOffEvent );
   DUMP_IPC_EVENTS;

   base::application::Thread::Configuration::tVector services =
   {
        { "OnOff_Service", { controller::components::onoff::Component::creator }, 5 }
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

bool test( int argc, char* argv[ ] );



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





#include "api/sys/oswrappers/Socket.hpp"

bool test( int argc, char* argv[ ] )
{
   return true;



   base::os::linux::socket::configuration confguration_core{ AF_UNIX, SOCK_STREAM, 0, "/tmp/core.socket", 5000 };
   base::os::linux::socket::configuration confguration_controller{ AF_UNIX, SOCK_STREAM, 0, "/tmp/controller.socket", 5000 };

   base::os::Socket::tSptr mp_socket_master = std::shared_ptr< base::os::Socket >( new base::os::Socket( confguration_controller, 4096 ) );
   if( base::os::Socket::eResult::ERROR == mp_socket_master->create( ) )
      return false;
   if( base::os::Socket::eResult::ERROR == mp_socket_master->bind( ) )
      return false;
   mp_socket_master->unblock( );
   if( base::os::Socket::eResult::ERROR == mp_socket_master->listen( ) )
      return false;
   mp_socket_master->info( "Server socket created" );



   base::os::Socket::tSptr mp_client_socket = std::shared_ptr< base::os::Socket >( new base::os::Socket( confguration_core, 4096 ) );
   if( base::os::Socket::eResult::ERROR == mp_client_socket->create( ) )
      return false;
   while( base::os::Socket::eResult::ERROR == mp_client_socket->connect( ) )
      sleep( 1 );
   mp_client_socket->unblock( );
   mp_client_socket->info( "Client socket created" );



   base::os::Socket::tSptrSet m_pending_sockets;





   while( true )
   {
      // prepare select
      DBG_TRC( "prepare select" );
      base::os::linux::socket::fd fd_set;
      fd_set.reset( );
      fd_set.set( mp_socket_master->socket( ), base::os::linux::socket::fd::eType::READ );
      base::os::linux::socket::tSocket max_socket = mp_socket_master->socket( );

      fd_set.set( mp_client_socket->socket( ), base::os::linux::socket::fd::eType::READ );
      if( mp_client_socket->socket( ) > max_socket )
         max_socket = mp_client_socket->socket( );

      for( const auto& p_socket : m_pending_sockets )
      {
         fd_set.set( p_socket->socket( ), base::os::linux::socket::fd::eType::READ );
         if( p_socket->socket( ) > max_socket )
            max_socket = p_socket->socket( );
      }
      SYS_INF( "max_socket = %d", max_socket );  

      // select
      DBG_TRC( "select" );
      if( false == base::os::linux::socket::select( max_socket, fd_set ) )
         continue;

      // process select
      DBG_TRC( "process select" );
      for( auto iterator = m_pending_sockets.begin( ); iterator != m_pending_sockets.end( ); ++iterator )
      {
         auto p_socket = *iterator;
         if( false == fd_set.is_set( p_socket->socket( ), base::os::linux::socket::fd::eType::READ ) )
            continue;

         const base::os::Socket::eResult result = p_socket->recv( );
         if( base::os::Socket::eResult::DISCONNECTED == result )
         {
            p_socket->info( "Client disconnected" );

            iterator = m_pending_sockets.erase( iterator );
            if( m_pending_sockets.end( ) == iterator )
               break;
         }
         else if( base::os::Socket::eResult::OK == result )
         {
            DBG_MSG( "reading pending socket" );
            size_t recv_size = 0;
            const void* const p_buffer = p_socket->buffer( recv_size );
         }
      }

      if( true == fd_set.is_set( mp_socket_master->socket( ), base::os::linux::socket::fd::eType::READ ) )
      {
         if( auto p_socket = mp_socket_master->accept( ) )
         {
            m_pending_sockets.insert( p_socket );
            p_socket->info( "Client connected" );
            p_socket->unblock( );
         }
      }

      if( true == fd_set.is_set( mp_client_socket->socket( ), base::os::linux::socket::fd::eType::READ ) )
      {
         const base::os::Socket::eResult result = mp_client_socket->recv( );
         if( base::os::Socket::eResult::DISCONNECTED == result )
         {
            mp_client_socket->info( "Client socket disconnected" );
         }
         else if( base::os::Socket::eResult::OK == result )
         {
            DBG_MSG( "reading client socket" );
            size_t recv_size = 0;
            const void* const p_buffer = mp_client_socket->buffer( recv_size );
         }
      }
   }




 


   return false;
}
