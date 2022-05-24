#pragma once

#include "carpc/events/Events.hpp"



namespace carpc::application {

   class SendReceive;



   class ServiceEventConsumer : public events::service::Action::Consumer
   {
      public:
         ServiceEventConsumer( SendReceive* const );
         ~ServiceEventConsumer( ) override;
      private:
         ServiceEventConsumer( const ServiceEventConsumer& ) = delete;
         ServiceEventConsumer& operator=( const ServiceEventConsumer& ) = delete;

      private:
         void process_event( const events::service::Action::Event& ) override;

      private:
         SendReceive* mp_send_receive;
   };

} // namespace carpc::application
