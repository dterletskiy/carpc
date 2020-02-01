#pragma once

#include <memory>



namespace base {

   class Server;

   class Client;

   class InterfaceRegistry;
   using InterfaceRegistryPtr = std::shared_ptr< InterfaceRegistry >;
   extern const InterfaceRegistryPtr InvalidInterfaceRegistryPtr;

   class InterfaceStatusHandler;
   using InterfaceStatusHandlerPtr = std::shared_ptr< InterfaceStatusHandler >;
   extern const InterfaceStatusHandlerPtr InvalidInterfaceStatusHandlerPtr;

} // namespace base
