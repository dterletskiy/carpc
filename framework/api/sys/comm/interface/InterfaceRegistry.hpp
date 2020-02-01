#pragma once

#include <set>

#include "api/sys/comm/interface/Types.hpp"



namespace base {



class InterfaceRegistry
{
public:
   ~InterfaceRegistry( );
   static InterfaceRegistryPtr instance( );
private:
   InterfaceRegistry( );
   InterfaceRegistry( const InterfaceRegistry& ) = delete;
   InterfaceRegistry& operator=( const InterfaceRegistry& ) = delete;
   static InterfaceRegistryPtr mp_instance;

public:
   bool register_server( const Server* const );
   bool register_client( const Client* const );
public:
   bool unregister_server( const Server* const );
   bool unregister_client( const Client* const );
};



} // namespace base
