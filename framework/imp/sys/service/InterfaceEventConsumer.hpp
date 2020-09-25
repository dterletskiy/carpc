#pragma once

#include "api/sys/events/Events.hpp"



namespace base {

   class ServiceIpcThread;

   class InterfaceEventConsumer : public events::interface::Action::Consumer
   {
      public:
         InterfaceEventConsumer( ServiceIpcThread& );
         ~InterfaceEventConsumer( ) override;
      private:
         InterfaceEventConsumer( const InterfaceEventConsumer& ) = delete;
         InterfaceEventConsumer& operator=( const InterfaceEventConsumer& ) = delete;

      private:
         void process_event( const events::interface::Action::Event& ) override;

      private:
         ServiceIpcThread& m_service;
   };

} // namespace base
