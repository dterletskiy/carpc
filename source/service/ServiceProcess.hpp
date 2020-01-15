#pragma once

#include "oswrappers/linux/timer/Timer.hpp"
#include "IServiceProcess.hpp"



namespace base {



class ServiceProcess
   : public IServiceProcess
{
public:
   ~ServiceProcess( ) override;
   static IServiceProcessPtr instance( );
private:
   ServiceProcess( );
   ServiceProcess( const ServiceProcess& ) = delete;
   ServiceProcess& operator=( const ServiceProcess& ) = delete;
   static IServiceProcessPtr mp_instance;

public:
   bool start( const ServiceInfoVector& ) override;
   bool stop( ) override;
   void boot( ) override;

public:
   IServiceBrockerDsiPtr service_brocker_dsi( ) const override;
private:
   IServiceBrockerDsiPtr   mp_service_brocker_dsi = InvalidServiceBrockerDsiPtr;

public:
   IServiceBrockerPtr service_brocker( ) const override;
private:
   IServiceBrockerPtr   mp_service_brocker = InvalidServiceBrockerPtr;

public:
   IServicePtr service( const TID& ) const override;
   IServicePtr current_service( ) const override;
   IServicePtrList service_list( ) const override;
private:
   IServicePtrList      m_service_list;

private:
   os::linux::TimerID   m_timer_id;
};





} // namespace base
