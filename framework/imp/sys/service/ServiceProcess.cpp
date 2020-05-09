#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/service/ServiceThread.hpp"
#include "api/sys/service/ServiceIpcThread.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/events/Events.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "SrvcProc"



namespace {

   void timer_handler( union sigval sv )
   {
      timer_t* timer_id = static_cast< timer_t* >( sv.sival_ptr );
      SYS_INF( "WhatchDog timer: %#lx", (long) *timer_id );

      for( const auto& p_service : base::ServiceProcess::instance( )->service_list( ) )
      {
         std::optional< time_t > time_stamp = p_service->process_started( );
         if( std::nullopt == time_stamp )
            continue;

         if( p_service->wd_timeout( ) > static_cast< size_t >( time( nullptr ) - time_stamp.value( ) ) )
            continue;

         SYS_ERR( "WhatchDog error: '%s'", p_service->name( ).c_str( ) );
      }
   }
}



using namespace base;



ServiceProcess::tSptr ServiceProcess::mp_instance = nullptr;

ServiceProcess::ServiceProcess( )
   : m_service_list( )
{
   SYS_TRC( "created" );
}

ServiceProcess::~ServiceProcess( )
{
   SYS_TRC( "destroyed" );
}

namespace { os::Mutex s_mutex; }
ServiceProcess::tSptr ServiceProcess::instance( )
{
   base::os::MutexAutoLocker locker( s_mutex );
   if( nullptr == mp_instance )
      mp_instance.reset( new ServiceProcess( ) );

   return mp_instance;
}

IServiceThread::tSptr ServiceProcess::service_ipc( ) const
{
   return mp_service_ipc;
}

IServiceThread::tSptr ServiceProcess::service( const TID& id ) const
{
   for( auto& p_service : m_service_list )
   {
      if( id == p_service->id( ) )
         return p_service;
   }

   return nullptr;
}

IServiceThread::tSptr ServiceProcess::current_service( ) const
{
   for( auto& p_service : m_service_list )
   {
      if( os::Thread::current_id( ) == p_service->id( ) )
         return p_service;
   }

   if( os::Thread::current_id( ) == mp_service_ipc->id( ) )
      return mp_service_ipc;

   return nullptr;
}

IServiceThread::tSptrList ServiceProcess::service_list( ) const
{
   return m_service_list;
}

bool ServiceProcess::start( const ServiceThread::Info::tVector& service_infos )
{
   REGISTER_EVENT( base::events::interface::Interface );

   // Creating service brocker thread
   mp_service_ipc = ServiceIpcThread::instance( );
   if( nullptr == mp_service_ipc )
      return false;
   // Creating service threads
   for( const auto& service_info : service_infos )
   {
      IServiceThread::tSptr p_service = std::make_shared< ServiceThread >( service_info );
      if( nullptr == p_service )
         return false;
      m_service_list.emplace_back( p_service );
   }

   // Starting service brocker thread
   if( false == mp_service_ipc->start( ) )
   {
      // return false;
   }
   sleep(1); // timeout IPC service thread is started before other service threads
   // Starting service threads
   for( const auto p_service : m_service_list )
      if( false == p_service->start( ) )
         return false;
   sleep(1); // timeout all service threads is started before booting system

   // Watchdog timer
   if( false == os::linux::timer::create( m_timer_id, timer_handler ) )
      return false;
   if( false == os::linux::timer::start( m_timer_id, 1000000000, os::linux::timer::eTimerType::continious ) )
      return false;

   return true;
}

bool ServiceProcess::stop( )
{
   for( auto& p_service : m_service_list )
      p_service->stop( );
   m_service_list.clear( );
   mp_service_ipc->stop( );

   os::linux::timer::remove( m_timer_id );

   return true;
}

void ServiceProcess::boot( )
{
   SYS_MSG( "booting..." );

   events::service::Service::Event::create_send( { events::service::eID::boot }, { "booting application" }, eCommType::ITC );
   events::service::Service::Event::create_send( { events::service::eID::boot }, { "booting application" }, eCommType::IPC );

   for( auto& p_service : m_service_list )
      p_service->wait( );
   SYS_MSG( "All services are stopped" );

   mp_service_ipc->wait( );
   SYS_MSG( "IPC service is stopped" );

   os::linux::timer::remove( m_timer_id );

   m_service_list.clear( );
   mp_service_ipc.reset( );
}
