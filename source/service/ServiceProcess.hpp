#pragma once

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
   bool init( const ServiceInfoVector& ) override;
   void boot( ) override;

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
};





} // namespace base
