#pragma once

#include "api/sys/oswrappers/Types.hpp"
#include "api/sys/oswrappers/linux/timer.hpp"
#include "api/sys/service/Types.hpp"



namespace base {



class ServiceProcess
   : public std::enable_shared_from_this< ServiceProcess >
{
public:
   ~ServiceProcess( );
   static ServiceProcessPtr instance( );
private:
   ServiceProcess( );
   ServiceProcess( const ServiceProcess& ) = delete;
   ServiceProcess& operator=( const ServiceProcess& ) = delete;
   static ServiceProcessPtr mp_instance;

public:
   bool start( const ServiceInfoVector& );
   bool stop( );
   void boot( );

public:
   ServiceBrockerPtr service_brocker( ) const;
private:
   ServiceBrockerPtr       mp_service_brocker = InvalidServiceBrockerPtr;

public:
   ServicePtr service( const TID& ) const;
   ServicePtr current_service( ) const;
   ServicePtrList service_list( ) const;
private:
   ServicePtrList          m_service_list;

private:
   os::linux::timer::TimerID  m_timer_id;
};





} // namespace base
