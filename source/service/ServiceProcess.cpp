#include <sstream>

#include "oswrappers/Mutex.hpp"
#include "ServiceBrockerDSI.hpp"
#include "ServiceBrocker.hpp"
#include "Service.hpp"
#include "ServiceProcess.hpp"

#include "Trace.hpp"
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



IServiceProcessPtr ServiceProcess::mp_instance;

ServiceProcess::ServiceProcess( )
   : m_service_list( )
{
   SYS_INF( "Created" );
}

ServiceProcess::~ServiceProcess( )
{
   SYS_INF( "Destroyed" );
}

IServiceProcessPtr ServiceProcess::instance( )
{
   os::Mutex mutex( true );
   if( !mp_instance )
   {
      mp_instance.reset( new ServiceProcess( ) );
   }

   return mp_instance;
}

IServiceBrockerDsiPtr ServiceProcess::service_brocker_dsi( ) const
{
   return mp_service_brocker_dsi;
}

IServiceBrockerPtr ServiceProcess::service_brocker( ) const
{
   return mp_service_brocker;
}

IServicePtr ServiceProcess::service( const TID& id ) const
{
   SYS_INF( "Service TID: %#x", id );
   for( auto& p_service : m_service_list )
   {
      SYS_INF( "Processing service TID: %#x", p_service->id( ) );
      if( id == p_service->id( ) )
         return p_service;
   }

   return InvalidServicePtr;
}

IServicePtr ServiceProcess::current_service( ) const
{
   for( auto& p_service : m_service_list )
   {
      if( os::Thread::current_id( ) == p_service->id( ) )
         return p_service;
   }

   return InvalidServicePtr;
}

IServicePtrList ServiceProcess::service_list( ) const
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
      IServicePtr p_service = std::make_shared< Service >( mp_service_brocker, service_info );
      if( nullptr == p_service )
         return false;
      m_service_list.emplace_back( p_service );
   }

   // Starting service brocker DSI thread
   if( false == mp_service_brocker_dsi->start( ) )
      return false;
   // Starting service brocker thread
   if( false == mp_service_brocker->start( ) )
      return false;
   // Starting service threads
   for( const auto p_service : m_service_list )
      if( false == p_service->start( ) )
         return false;

   // Watchdog timer
   if( false == os::linux::create_timer( m_timer_id, timer_handler ) )
      return false;
   DBG_MSG( "Timer ID: %#lx", (long) m_timer_id );
   if( false == os::linux::start_timer( m_timer_id, 1000000000, os::linux::eTimerType::continious ) )
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

   os::linux::delete_timer( m_timer_id );

   return true;
}

void ServiceProcess::boot( )
{
   SYS_MSG( );

   // ServiceEvent::send_event( { eServiceCommand::boot, "boot" }, eCommType::ITC );
   DsiService::DsiServiceEvent::send_event( { base::eServiceCommand::boot, "fuck" } );



   for( auto& p_service : m_service_list )
      p_service->wait( );
   DBG_MSG( "All services are finished" );
   DBG_MSG( "Stopping Service Brocker" );
   ServiceEvent::send_event( { eServiceCommand::ping, "ping" }, eCommType::ITC );
   mp_service_brocker->stop( );
   mp_service_brocker->wait( );
   DBG_MSG( "Service Brocker is finished" );
   DBG_MSG( "Stopping Service Brocker DSI" );
   mp_service_brocker_dsi->stop( );
   mp_service_brocker_dsi->wait( );
   DBG_MSG( "Service Brocker DSI is finished" );

   os::linux::delete_timer( m_timer_id );

   m_service_list.clear( );
   mp_service_brocker.reset( );
   mp_service_brocker_dsi.reset( );
}



} // namespace base
