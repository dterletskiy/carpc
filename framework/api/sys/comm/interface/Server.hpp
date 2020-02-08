#pragma once

#include "api/sys/comm/interface/Interface.hpp"



namespace base {



class Client;



class Server : public Interface
{
public:
   Server( const std::string&, const std::string&, const bool );
   ~Server( ) override;

public:
   void connected( const Client* const ) const;
   void disconnected( const Client* const ) const;

public:
   const bool busy( const bool );
   const bool busy( ) const;
private:
   bool m_busy = false;

private:
   mutable const Client* mp_client = nullptr;
};



inline
const bool Server::busy( const bool _busy )
{
   if( m_busy && _busy )
      return true;

   m_busy = _busy;
   return false;
}

inline
const bool Server::busy( ) const
{
   return m_busy;
}



} // namespace base
