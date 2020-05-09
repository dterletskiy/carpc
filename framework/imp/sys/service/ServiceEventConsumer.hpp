#pragma once

#include "api/sys/events/Events.hpp"



namespace base {

   class IServiceThread;

   class ServiceEventConsumer : public events::service::Service::Consumer
   {
      public:
         ServiceEventConsumer( IServiceThread& );
         ~ServiceEventConsumer( ) override;
      private:
         ServiceEventConsumer( const ServiceEventConsumer& ) = delete;
         ServiceEventConsumer& operator=( const ServiceEventConsumer& ) = delete;

      private:
         IServiceThread& m_service;

      private:
         void process_event( const events::service::Service::Event& ) override;
   };

} // namespace base
