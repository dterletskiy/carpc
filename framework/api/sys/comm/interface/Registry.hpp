#pragma once

#include <set>

#include "api/sys/comm/interface/Types.hpp"



namespace base {



class Registry
{
public:
   ~Registry( );
   static RegistryPtr instance( );
private:
   Registry( );
   static RegistryPtr mp_instance;

public:
   bool register_server( const Server* const );
   bool register_client( const Client* const );
public:
   bool unregister_server( const Server* const );
   bool unregister_client( const Client* const );

private:
   std::set< const Server* > m_server_registry;
   std::set< const Client* > m_client_registry;
};



} // namespace base
