#pragma once

#include "api/sys/events/Events.hpp"



namespace base {

   class IServiceThread;

   class InterfaceEventConsumer : public events::interface::Interface::Consumer
   {
      public:
         InterfaceEventConsumer( IServiceThread& );
         ~InterfaceEventConsumer( ) override;
      private:
         InterfaceEventConsumer( const InterfaceEventConsumer& ) = delete;
         InterfaceEventConsumer& operator=( const InterfaceEventConsumer& ) = delete;

      private:
         IServiceThread& m_service;

      private:
         void process_event( const events::interface::Interface::Event& ) override;
   };

} // namespace base
