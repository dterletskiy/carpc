#include <sstream>

#include "oswrappers/Mutex.hpp"
#include "oswrappers/linux/timer/Timer.hpp"
#include "ServiceBrocker.hpp"
#include "Service.hpp"
#include "ServiceProcess.hpp"

#include "Trace.hpp"
#define CLASS_ABBR "SrvcProc"



namespace base {



IServiceProcessPtr ServiceProcess::mp_instance;

ServiceProcess::ServiceProcess( )
   : mp_service_brocker( )
   , m_service_list( )
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

bool ServiceProcess::init( const ServiceInfoVector& service_infos )
{
   // Creating service brocker thread
   mp_service_brocker = ServiceBrocker::instance( );

   // Creating service threads
   for( const auto& service_info : service_infos )
   {
      IServicePtr p_service = std::make_shared< Service >( mp_service_brocker, service_info );
      m_service_list.emplace_back( p_service );
   }

   // Starting service brocker thread
   if( false == mp_service_brocker->start( ) )
      return false;

   // Starting service threads
   for( const auto p_service : m_service_list )
      p_service->start( );

   if( service_infos.size( ) != m_service_list.size( ) )
   {
      for( auto& p_service : m_service_list )
         p_service->stop( );
      m_service_list.clear( );
      mp_service_brocker->stop( );
   }

   return !m_service_list.empty( );
}

void event_handler( union sigval sv )
{
   timer_t* timer_id = static_cast< timer_t* >( sv.sival_ptr );
   SYS_INF( "WhatchDog timer: %#lx", (long) *timer_id );

   for( const auto& p_service : ServiceProcess::instance( )->service_list( ) )
   {
      std::optional< time_t > time_stamp = p_service->process_started( );
      if( std::nullopt == time_stamp )
         continue;

      if( p_service->wd_timeout( ) > static_cast< size_t >( time( nullptr ) - time_stamp.value( ) ) )
         continue;

      SYS_ERR( "WhatchDog error: '%s'", p_service->name( ).c_str( ) );
   }
}

void ServiceProcess::boot( )
{
   SYS_MSG( );

   // testing
   os::linux::TimerID   timer_id;
   if( false == os::linux::create_timer( timer_id, event_handler ) )
      return;
   DBG_MSG( "Timer ID: %#lx", (long) timer_id );
   if( false == os::linux::start_timer( timer_id, 1000000000, os::linux::eTimerType::continious ) )
      return;

   sleep( 1 );
   ServiceEvent::send_event( { eServiceCommand::boot, "boot" }, eCommType::ITC );

#if 0
   sleep( 5 );
   DBG_MSG( "---------- boot ----------" );
   ServiceEvent::send_event( { eServiceCommand::boot, "boot" }, eCommType::ITC );
   sleep( 1 );

   DBG_MSG( "---------- ping ----------" );
   ServiceEvent::send_event( { eServiceCommand::ping, "first ping" }, eCommType::ITC );
   sleep( 1 );
   DBG_MSG( "---------- ping ----------" );
   ServiceEvent::send_event( { eServiceCommand::ping, "second ping" }, eCommType::ITC );
   sleep( 1 );
   DBG_MSG( "---------- ping ----------" );
   ServiceEvent::send_event( { eServiceCommand::ping, "third ping" }, eCommType::ITC );
   sleep( 1 );

   DBG_MSG( "---------- shutdown ----------" );
   ServiceEvent::send_event( { eServiceCommand::shutdown, "shutdown" }, eCommType::ITC );
   DBG_MSG( "-------------------------" );
#endif

   for( auto& p_service : m_service_list )
      p_service->wait( );
   DBG_MSG( "All services are finished" );
   DBG_MSG( "Stopping Service Brocker" );
   ServiceEvent::send_event( { eServiceCommand::ping, "ping" }, eCommType::ITC );
   mp_service_brocker->stop( );
   mp_service_brocker->wait( );
   DBG_MSG( "Service Brocker is finished" );

   os::linux::delete_timer( timer_id );

   m_service_list.clear( );
   mp_service_brocker.reset( );
}



} // namespace base
