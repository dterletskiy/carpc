#pragma once

#include <string>

#include "api/sys/service/Types.hpp"



namespace base {



class Server;
class Client;



class Interface
{
public:
   enum class eType { server, client, undefined };

public:
   Interface( const std::string&, const std::string&, const eType );
   virtual ~Interface( );

public:
   const std::string& name( ) const;
   const std::string& role( ) const;
   const std::string service_name( ) const;
   const bool cmp_names( const Interface* const ) const;
private:
   std::string m_name = "";
   std::string m_role = "";

public:
   bool is_connected( ) const;
   virtual void connected( const Interface* const ) const = 0;
   virtual void disconnected( const Interface* const ) const = 0;
private:
   bool m_is_connected = false;

public:
   eType type( ) const;
private:
   eType m_type = eType::undefined;

protected:
   const bool register_server( const Server* const );
   const bool register_client( const Client* const );
   const bool unregister_server( const Server* const );
   const bool unregister_client( const Client* const );

public:
   ServiceThreadPtrW context( ) const;
private:
   ServiceThreadPtrW mp_service_thread;
};



inline
const std::string& Interface::name( ) const
{
   return m_name;
}

inline
const std::string& Interface::role( ) const
{
   return m_role;
}

inline
const std::string Interface::service_name( ) const
{
   return m_role + std::string{"."} + m_name;
}

inline
const bool Interface::cmp_names( const Interface* const p_interface ) const
{
   return ( p_interface->name( ) == m_name ) && ( p_interface->role( ) == m_role );
}

inline
bool Interface::is_connected( ) const
{
   return m_is_connected;
}

inline
Interface::eType Interface::type( ) const
{
   return m_type;
}

inline
ServiceThreadPtrW Interface::context( ) const
{
   return mp_service_thread;
}



} // namespace base
