#pragma once

#include "api/sys/service/IServiceThread.hpp"
#include "imp/sys/events/Events.hpp"



namespace base {



class ServiceEventConsumer
   : public events::service::ServiceEvent::Consumer
{
public:
   ServiceEventConsumer( IServiceThread::tSptr );
   ~ServiceEventConsumer( ) override;
private:
   ServiceEventConsumer( const ServiceEventConsumer& ) = delete;
   ServiceEventConsumer& operator=( const ServiceEventConsumer& ) = delete;

private:
   IServiceThread::tWptr mp_service;

private:
   void process_event( const events::service::ServiceEvent::Event& ) override;
};




} // namespace base
