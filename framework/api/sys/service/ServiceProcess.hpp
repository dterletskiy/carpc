#pragma once

#include "api/sys/oswrappers/Types.hpp"
#include "api/sys/oswrappers/linux/timer.hpp"
#include "api/sys/service/ServiceBrockerThread.hpp"
#include "api/sys/service/ServiceThread.hpp"



namespace base {



class ServiceProcess
   : public std::enable_shared_from_this< ServiceProcess >
{
public:
   using tSptr = std::shared_ptr< ServiceProcess >;
   using tWptr = std::weak_ptr< ServiceProcess >;
   using tUptr = std::unique_ptr< ServiceProcess >;
   using tRptr = ServiceProcess*;

public:
   ~ServiceProcess( );
   static tSptr instance( );
private:
   ServiceProcess( );
   ServiceProcess( const ServiceProcess& ) = delete;
   ServiceProcess& operator=( const ServiceProcess& ) = delete;
   static tSptr mp_instance;

public:
   bool start( const ServiceThread::Info::tVector& );
   bool stop( );
   void boot( );

public:
   ServiceBrockerThread::tSptr service_brocker( ) const;
private:
   ServiceBrockerThread::tSptr   mp_service_brocker = nullptr;

public:
   IServiceThread::tSptr service( const TID& ) const;
   IServiceThread::tSptr current_service( ) const;
   IServiceThread::tSptrList service_list( ) const;
private:
   IServiceThread::tSptrList  m_service_list;

private:
   os::linux::timer::TimerID  m_timer_id;
};





} // namespace base
