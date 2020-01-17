#pragma once

#include <string>

#include "api/sys/service/Types.hpp"



namespace base {



class Interface
{
public:
   enum class eType { server, client, undefined };

public:
   Interface( const std::string&, const std::string&, const eType );
   virtual ~Interface( );

public:
   const std::string& name( ) const;
private:
   std::string m_name = "";

public:
   const std::string& role( ) const;
private:
   std::string m_role = "";

public:
   bool is_connected( ) const;
   virtual void connected( ) const = 0;
   virtual void disconnected( ) const = 0;
private:
   bool m_is_connected = false;

public:
   eType type( ) const;
private:
   eType m_type = eType::undefined;

public:
   ServicePtrW service( ) const;
private:
   ServicePtrW mp_service;
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
bool Interface::is_connected( ) const
{
   return m_is_connected;
}

inline
Interface::eType Interface::type( ) const
{
   return m_type;
}



} // namespace base
