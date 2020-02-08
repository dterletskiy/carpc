#pragma once

#include "api/sys/comm/interface/Interface.hpp"



namespace base {



class Server;



class Client : public Interface
{
public:
   Client( const std::string&, const std::string&, const bool );
   ~Client( ) override;

public:
   void connected( const Server* const ) const;
   void disconnected( const Server* const ) const;

private:
   mutable const Server* mp_server = nullptr;
};



} // namespace base
