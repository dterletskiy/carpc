#include <sys/types.h>
#include <unistd.h>
#include <filesystem>
#include <chrono>
#include <thread>
#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/application/Thread.hpp"
#include "api/sys/application/ThreadIPC.hpp"
#include "api/sys/application/Process.hpp"
#include "api/sys/events/Events.hpp"
#include "api/sys/tools/Tools.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "SrvcProc"



namespace {

   void timer_handler( union sigval sv )
   {
      timer_t* timer_id = static_cast< timer_t* >( sv.sival_ptr );
      SYS_INF( "WhatchDog timer: %#lx", (long) *timer_id );

      for( const auto& p_thread : base::application::Process::instance( )->thread_list( ) )
      {
         time_t time_stamp = p_thread->process_started( );
         if( 0 == time_stamp )
            continue;

         if( p_thread->wd_timeout( ) > static_cast< size_t >( time( nullptr ) - time_stamp ) )
            continue;

         SYS_ERR( "WhatchDog error: '%s'", p_thread->name( ).c_str( ) );
      }
   }
}



using namespace base::application;



Process::tSptr Process::mp_instance = nullptr;

Process::Process( int argc, char** argv )
   : m_id( getpid( ) ) // @TDA: should be unique for all computers in the network
{
   SYS_TRC( "created" );

   std::string file_name = std::filesystem::path( *argv ).filename( );
   file_name += std::string( ".cfg" );

   base::tools::cmd::init( argc, argv );
   base::tools::cmd::print( );
   base::tools::cfg::init( base::tools::cmd::argument( "config" ).value_or( file_name.c_str( ) ) );
   base::tools::cfg::print( );

   m_configuration.ipc_sb = dsi::SocketCongiguration {
      AF_UNIX,
      SOCK_STREAM,
      static_cast< int >( std::stoll( base::tools::cfg::argument( "ipc_servicebrocker_protocole" ).value( ) ) ),
      base::tools::cfg::argument( "ipc_servicebrocker_address" ).value( ),
      static_cast< int >( std::stoll( base::tools::cfg::argument( "ipc_servicebrocker_port" ).value( ) ) )
   };
   m_configuration.ipc_sb_buffer_size = static_cast< size_t >( std::stoll( base::tools::cfg::argument( "ipc_servicebrocker_buffer_size" ).value( ) ) );

   m_configuration.ipc_app = dsi::SocketCongiguration {
      AF_UNIX,
      SOCK_STREAM,
      static_cast< int >( std::stoll( base::tools::cfg::argument( "ipc_application_protocole" ).value( ) ) ),
      base::tools::cfg::argument( "ipc_application_address" ).value( ),
      static_cast< int >( std::stoll( base::tools::cfg::argument( "ipc_application_port" ).value( ) ) )
   };
   m_configuration.ipc_app_buffer_size = static_cast< size_t >( std::stoll( base::tools::cfg::argument( "ipc_application_buffer_size" ).value( ) ) );
}

Process::~Process( )
{
   SYS_TRC( "destroyed" );
}

namespace { base::os::Mutex s_mutex; }
Process::tSptr Process::instance( int argc, char** argv )
{
   // First call of this function must be done from main function => should not be race condition.
   // os::MutexAutoLocker locker( s_mutex );
   if( nullptr == mp_instance )
   {
      if( argc < 1 || nullptr == argv )
      {
         SYS_ERR( "called not from 'main( int argc, char** argv )' function" );
         return nullptr;
      }
      mp_instance.reset( new Process( argc, argv ) );
   }

   return mp_instance;
}

IThread::tSptr Process::thread_ipc( ) const
{
   return mp_thread_ipc;
}

IThread::tSptr Process::thread( const thread::ID& id ) const
{
   for( auto& p_thread : m_thread_list )
   {
      if( id == p_thread->id( ) )
         return p_thread;
   }

   return nullptr;
}

IThread::tSptr Process::current_thread( ) const
{
   for( auto& p_thread : m_thread_list )
   {
      if( os::Thread::current_id( ) == p_thread->thread( ).id( ) )
         return p_thread;
   }

   if( os::Thread::current_id( ) == mp_thread_ipc->thread( ).id( ) )
      return mp_thread_ipc;

   return nullptr;
}

bool Process::start( const Thread::Configuration::tVector& thread_configs )
{
   // Creating IPC brocker thread
   mp_thread_ipc = std::make_shared< ThreadIPC >( );
   if( nullptr == mp_thread_ipc )
      return false;
   // Creating application threads
   for( const auto& thread_config : thread_configs )
   {
      IThread::tSptr p_thread = std::make_shared< Thread >( thread_config );
      if( nullptr == p_thread )
         return false;
      m_thread_list.emplace_back( p_thread );
   }

   // Starting IPC brocker thread
   if( false == mp_thread_ipc->start( ) )
   {
      // return false;
   }
   std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) ); // timeout IPC thread is started before other application threads
   // Starting application threads
   for( const auto p_thread : m_thread_list )
      if( false == p_thread->start( ) )
         return false;
   std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) ); // timeout all application threads is started before booting system

   // Watchdog timer
   if( false == os::linux::timer::create( m_timer_id, timer_handler ) )
      return false;
   if( false == os::linux::timer::start( m_timer_id, 1000000000, os::linux::timer::eTimerType::continious ) )
      return false;

   return true;
}

bool Process::stop( )
{
   for( auto& p_thread : m_thread_list )
      p_thread->stop( );
   m_thread_list.clear( );
   mp_thread_ipc->stop( );

   os::linux::timer::remove( m_timer_id );

   return true;
}

void Process::boot( )
{
   SYS_MSG( "booting..." );

   events::system::System::Event::create_send( { events::system::eID::boot }, { "booting application" } );

   for( auto& p_thread : m_thread_list )
      p_thread->wait( );
   SYS_MSG( "All application threads are stopped" );

   mp_thread_ipc->wait( );
   SYS_MSG( "IPC thread is stopped" );

   os::linux::timer::remove( m_timer_id );

   m_thread_list.clear( );
   mp_thread_ipc.reset( );
}
