#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/service/ServiceBrockerDSI.hpp"
#include "api/sys/service/ServiceBrocker.hpp"
#include "api/sys/service/Service.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "imp/sys/events/ServiceEvent.hpp"

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



namespace base {



ServiceProcessPtr ServiceProcess::mp_instance;

ServiceProcess::ServiceProcess( )
   : m_service_list( )
{
   SYS_INF( "Created" );
}

ServiceProcess::~ServiceProcess( )
{
   SYS_INF( "Destroyed" );
}

ServiceProcessPtr ServiceProcess::instance( )
{
   os::Mutex mutex( true );
   if( !mp_instance )
   {
      mp_instance.reset( new ServiceProcess( ) );
   }

   return mp_instance;
}

ServiceBrockerDsiPtr ServiceProcess::service_brocker_dsi( ) const
{
   return mp_service_brocker_dsi;
}

ServiceBrockerPtr ServiceProcess::service_brocker( ) const
{
   return mp_service_brocker;
}

ServicePtr ServiceProcess::service( const TID& id ) const
{
   SYS_INF( "Service TID: %#lx", id );
   for( auto& p_service : m_service_list )
   {
      SYS_INF( "Processing service TID: %#lx", p_service->id( ) );
      if( id == p_service->id( ) )
         return p_service;
   }

   return InvalidServicePtr;
}

ServicePtr ServiceProcess::current_service( ) const
{
   for( auto& p_service : m_service_list )
   {
      if( os::Thread::current_id( ) == p_service->id( ) )
         return p_service;
   }

   return InvalidServicePtr;
}

ServicePtrList ServiceProcess::service_list( ) const
{
   return m_service_list;
}

bool ServiceProcess::start( const ServiceInfoVector& service_infos )
{
   // Creating service brocker DSI thread
   mp_service_brocker_dsi = ServiceBrockerDSI::instance( );
   if( nullptr == mp_service_brocker_dsi )
      return false;
   // Creating service brocker thread
   mp_service_brocker = ServiceBrocker::instance( );
   if( nullptr == mp_service_brocker )
      return false;
   // Creating service threads
   for( const auto& service_info : service_infos )
   {
      ServicePtr p_service = std::make_shared< Service >( mp_service_brocker, service_info );
      if( nullptr == p_service )
         return false;
      m_service_list.emplace_back( p_service );
   }

   // Starting service brocker DSI thread
   if( false == mp_service_brocker_dsi->start( ) )
   {
      SYS_ERR( "DSI Service Brocker can't be started" );
      // return false;
   }
   // Starting service brocker thread
   if( false == mp_service_brocker->start( ) )
      return false;
   // Starting service threads
   for( const auto p_service : m_service_list )
      if( false == p_service->start( ) )
         return false;

   // Watchdog timer
   if( false == os::linux::timer::create( m_timer_id, timer_handler ) )
      return false;
   SYS_MSG( "Timer ID: %#lx", (long) m_timer_id );
   if( false == os::linux::timer::start( m_timer_id, 1000000000, os::linux::timer::eTimerType::continious ) )
      return false;

   return true;
}

bool ServiceProcess::stop( )
{
   for( auto& p_service : m_service_list )
      p_service->stop( );
   m_service_list.clear( );
   mp_service_brocker->stop( );
   mp_service_brocker_dsi->stop( );

   os::linux::timer::remove( m_timer_id );

   return true;
}

void ServiceProcess::boot( )
{
   SYS_INF( "booting..." );

   sleep(1);

   ServiceEvent::Event::create_send( { eServiceCommand::boot, "boot" }, eCommType::ITC );

   for( auto& p_service : m_service_list )
      p_service->wait( );
   SYS_MSG( "All services are finished" );
   SYS_MSG( "Stopping Service Brocker" );
   ServiceEvent::Event::create_send( { eServiceCommand::ping, "ping" }, eCommType::ITC );
   mp_service_brocker->stop( );
   mp_service_brocker->wait( );
   SYS_MSG( "Service Brocker is finished" );
   SYS_MSG( "Stopping Service Brocker DSI" );
   mp_service_brocker_dsi->stop( );
   mp_service_brocker_dsi->wait( );
   SYS_MSG( "Service Brocker DSI is finished" );

   os::linux::timer::remove( m_timer_id );

   m_service_list.clear( );
   mp_service_brocker.reset( );
   mp_service_brocker_dsi.reset( );
}



} // namespace base
