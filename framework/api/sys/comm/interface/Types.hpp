#pragma once

#include <memory>



namespace base {

   class Server;

   class Client;

   class Registry;
   using RegistryPtr = std::shared_ptr< Registry >;
   extern const RegistryPtr InvalidRegistryPtr;

} // namespace base
