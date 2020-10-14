#pragma once

#include "api/sys/events/Events.hpp"



namespace base::application {

   class ThreadIPC;



   class ServiceEventConsumer : public events::service::Action::Consumer
   {
      public:
         ServiceEventConsumer( ThreadIPC& );
         ~ServiceEventConsumer( ) override;
      private:
         ServiceEventConsumer( const ServiceEventConsumer& ) = delete;
         ServiceEventConsumer& operator=( const ServiceEventConsumer& ) = delete;

      private:
         void process_event( const events::service::Action::Event& ) override;

      private:
         ThreadIPC& m_service;
   };

} // namespace base::application
