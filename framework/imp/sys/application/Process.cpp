#include <sys/types.h>
#include <unistd.h>
#include <filesystem>
#include <chrono>
#include <thread>
#include "api/sys/oswrappers/linux/signals.hpp"
#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/application/Thread.hpp"
#include "api/sys/application/ThreadIPC.hpp"
#include "api/sys/application/Process.hpp"
#include "api/sys/events/Events.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "SrvcProc"



namespace {

   void process_watchdog( )
   {
      for( const auto& p_thread : carpc::application::Process::instance( )->thread_list( ) )
      {
         time_t time_stamp = p_thread->process_started( );
         if( 0 == time_stamp )
            continue;

         if( 0 >= p_thread->wd_timeout( ) )
            continue;
         if( p_thread->wd_timeout( ) > static_cast< std::size_t >( time( nullptr ) - time_stamp ) )
            continue;

         SYS_ERR( "WatchDog error: '%s'", p_thread->name( ).c_str( ) );
      }
   }

   void timer_handler( union sigval sv )
   {
      carpc::os::os_linux::timer::tID* timer_id = static_cast< carpc::os::os_linux::timer::tID* >( sv.sival_ptr );
      SYS_INF( "WatchDog timer: %#lx", (long) *timer_id );
      process_watchdog( );
   }

   void signal_handler( int signal, siginfo_t* si, void* uc )
   {
      carpc::os::os_linux::timer::tID* timer_id = static_cast< carpc::os::os_linux::timer::tID* >( si->si_value.sival_ptr );
      SYS_INF( "WatchDog timer: %#lx", (long) *timer_id );
      process_watchdog( );
   }

}



using namespace carpc::application;



Process::tSptr Process::mp_instance = nullptr;

Process::Process( int argc, char** argv, char** envp )
   : m_id( getpid( ) ) // @TDA: should be unique for all computers in the network
   , m_pce( argc, argv, envp )
{
   SYS_VRB( "created" );

   m_pce.print( );

   if( "true" == m_pce.value( "ipc" ).value_or( "true" ) )  m_configuration.ipc = true;
   else                                                     m_configuration.ipc = false;

   if(
            false == m_configuration.ipc
         || std::nullopt == m_pce.value( "ipc_servicebrocker_domain" )
         || std::nullopt == m_pce.value( "ipc_servicebrocker_type" )
         || std::nullopt == m_pce.value( "ipc_servicebrocker_protocole" )
         || std::nullopt == m_pce.value( "ipc_servicebrocker_address" )
         || std::nullopt == m_pce.value( "ipc_servicebrocker_port" )

         || std::nullopt == m_pce.value( "ipc_application_domain" )
         || std::nullopt == m_pce.value( "ipc_application_type" )
         || std::nullopt == m_pce.value( "ipc_application_protocole" )
         || std::nullopt == m_pce.value( "ipc_application_address" )
         || std::nullopt == m_pce.value( "ipc_application_port" )
      ) m_configuration.ipc = false;

   if( true == m_configuration.ipc )
   {
      m_configuration.ipc_sb.socket = os::os_linux::socket::configuration {
         carpc::os::os_linux::socket::socket_domain_from_string(
               m_pce.value( "ipc_servicebrocker_domain" ).value( ).c_str( )
            ),
         carpc::os::os_linux::socket::socket_type_from_string(
               m_pce.value( "ipc_servicebrocker_type" ).value( ).c_str( )
            ),
         static_cast< int >( std::stoll( m_pce.value( "ipc_servicebrocker_protocole" ).value( ) ) ),
         m_pce.value( "ipc_servicebrocker_address" ).value( ),
         static_cast< int >( std::stoll( m_pce.value( "ipc_servicebrocker_port" ).value( ) ) )
      };
      m_configuration.ipc_sb.buffer_size = static_cast< std::size_t >( std::stoll(
            m_pce.value( "ipc_servicebrocker_buffer_size" ).value_or( "4096" ) )
         );

      m_configuration.ipc_app.socket = os::os_linux::socket::configuration {
         carpc::os::os_linux::socket::socket_domain_from_string(
               m_pce.value( "ipc_application_domain" ).value( ).c_str( )
            ),
         carpc::os::os_linux::socket::socket_type_from_string(
               m_pce.value( "ipc_application_type" ).value( ).c_str( )
            ),
         static_cast< int >( std::stoll( m_pce.value( "ipc_application_protocole" ).value( ) ) ),
         m_pce.value( "ipc_application_address" ).value( ),
         static_cast< int >( std::stoll( m_pce.value( "ipc_application_port" ).value( ) ) )
      };
      m_configuration.ipc_app.buffer_size = static_cast< std::size_t >(
            std::stoll( m_pce.value( "ipc_application_buffer_size" ).value_or( "4096" ) )
         );
   }

   m_configuration.wd_timout = static_cast< std::size_t >( std::stoll( m_pce.value_or( "application_wd_timout", "10" ) ) );

   DUMP_IPC_EVENTS;
}

Process::~Process( )
{
   SYS_VRB( "destroyed" );
}

namespace { carpc::os::Mutex s_mutex; }
Process::tSptr Process::instance( int argc, char** argv, char** envp )
{
   // First call of this function must be done from main function => should not be race condition.
   // os::Mutex::AutoLocker locker( s_mutex );
   if( nullptr == mp_instance )
   {
      if( argc < 1 || nullptr == argv )
      {
         SYS_ERR( "called not from 'main( int argc, char** argv )' function" );
         return nullptr;
      }
      mp_instance.reset( new Process( argc, argv, envp ) );
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
      if( p_thread && os::Thread::current_id( ) == p_thread->thread( ).id( ) )
         return p_thread;
   }

   if( mp_thread_ipc && os::Thread::current_id( ) == mp_thread_ipc->thread( ).id( ) )
      return mp_thread_ipc;

   return nullptr;
}

bool Process::start( const Thread::Configuration::tVector& thread_configs )
{
   if( true == m_configuration.ipc )
   {
      // Creating IPC brocker thread
      mp_thread_ipc = std::make_shared< ThreadIPC >( );
      if( nullptr == mp_thread_ipc )
         return false;

      // Starting IPC brocker thread
      if( true == mp_thread_ipc->start( ) )
      {
         SYS_INF( "starting IPC thread started" );
         while( false == mp_thread_ipc->started( ) )
         {
            std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
            SYS_WRN( "waiting to strat IPC thread" );
         }
         SYS_INF( "IPC thread started" );
      }
      else
      {
         SYS_WRN( "starting without IPC thread" );
      }
   }
   else
   {
      SYS_INF( "starting without IPC thread" );
   }

   // Creating application threads
   for( const auto& thread_config : thread_configs )
   {
      IThread::tSptr p_thread = std::make_shared< Thread >( thread_config );
      if( nullptr == p_thread )
         return false;
      m_thread_list.emplace_back( p_thread );
   }

   // Starting application threads
   for( const auto& p_thread : m_thread_list )
   {
      SYS_INF( "starting '%s' thread", p_thread->name( ).c_str( ) );
      if( false == p_thread->start( ) )
         return false;
   }

   while( true )
   {
      bool threads_started = true;
      std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
      for( const auto& p_thread : m_thread_list )
      {
         bool thread_started = p_thread->started( );
         threads_started &= thread_started;
         if( false == thread_started )
         {
            SYS_WRN( "waiting to strat '%s' thread", p_thread->name( ).c_str( ) );
         }
      }

      if( true == threads_started )
         break;
   }
   SYS_INF( "all application threads started" );

   // Watchdog timer
   if( 0 < m_configuration.wd_timout )
   {
      if( false == os::os_linux::timer::create( m_timer_id, timer_handler ) )
         return false;
      if( false == os::os_linux::timer::start( m_timer_id, m_configuration.wd_timout * 1000000000, os::os_linux::timer::eTimerType::continious ) )
         return false;
   }
   else
   {
      SYS_WRN( "WatchDog disabled" );
   }

   return true;
}

bool Process::stop( )
{
   for( auto& p_thread : m_thread_list )
      if( p_thread )
         p_thread->stop( );
   m_thread_list.clear( );
   if( mp_thread_ipc )
      mp_thread_ipc->stop( );

   os::os_linux::timer::remove( m_timer_id );

   return true;
}

void Process::boot( )
{
   SYS_DBG( "booting..." );

   events::system::System::Event::create_send( { events::system::eID::boot }, { "booting application" } );

   for( auto& p_thread : m_thread_list )
      if( p_thread )
         p_thread->wait( );
   SYS_INF( "All application threads are stopped" );

   if( mp_thread_ipc )
      mp_thread_ipc->wait( );
   SYS_INF( "IPC thread is stopped" );

   os::os_linux::timer::remove( m_timer_id );

   m_thread_list.clear( );
   mp_thread_ipc.reset( );
}
