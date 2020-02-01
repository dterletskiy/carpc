#pragma once

#include "api/sys/comm/event/Event.hpp"



namespace base {

   DEFINE_ENUM( eInterfaceRegistry, size_t, ServerConnected, ServerDisconnected, ClientConnected, ClientDisconnected, Undefined );
   struct InterfaceRegistryEventData
   {
      const Server* p_server = nullptr;
      const Client* p_client = nullptr;
   };
   DECLARE_EVENT( InterfaceRegistryEvent, InterfaceRegistryEventData, eInterfaceRegistry );

} // namespace base



namespace base {

   class InterfaceStatusHandler : public InterfaceRegistryEvent::Consumer
   {
   public:
      InterfaceStatusHandler( );
      ~InterfaceStatusHandler( ) override;
   private:
      void process_event( const InterfaceRegistryEvent::Event& ) override;
   };

} // namespace base
