#pragma once

#include "api/sys/events/Events.hpp"



namespace base::application {

   class SendReceive;



   class ServiceEventConsumer : public events::service::Action::Consumer
   {
      public:
         ServiceEventConsumer( SendReceive& );
         ~ServiceEventConsumer( ) override;
      private:
         ServiceEventConsumer( const ServiceEventConsumer& ) = delete;
         ServiceEventConsumer& operator=( const ServiceEventConsumer& ) = delete;

      private:
         void process_event( const events::service::Action::Event& ) override;

      private:
         SendReceive& m_send_receive;
   };

} // namespace base::application
