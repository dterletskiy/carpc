#pragma once

#include "api/sys/events/Events.hpp"
#include "api/sys/comm/interface/IServer.hpp"



namespace base {

   class ServiceIpcThread;

   class InterfaceEventConsumer : public events::interface::Interface::Consumer
   {
      public:
         InterfaceEventConsumer( ServiceIpcThread& );
         ~InterfaceEventConsumer( ) override;
      private:
         InterfaceEventConsumer( const InterfaceEventConsumer& ) = delete;
         InterfaceEventConsumer& operator=( const InterfaceEventConsumer& ) = delete;

      private:
         void process_event( const events::interface::Interface::Event& ) override;

      private:
         ServiceIpcThread& m_service;

      private:
         std::list< std::string > m_local_servers;
         std::list< std::string > m_external_servers;
         std::list< std::string > m_local_clients;
         std::list< std::string > m_external_clients;
   };

} // namespace base
