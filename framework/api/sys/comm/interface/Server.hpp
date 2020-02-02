#pragma once

#include "api/sys/comm/interface/Interface.hpp"



namespace base {



class Server : public Interface
{
public:
   Server( const std::string&, const std::string&, const bool );
   ~Server( ) override;
};



} // namespace base
