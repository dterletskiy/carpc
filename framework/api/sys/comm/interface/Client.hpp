#pragma once

#include "api/sys/comm/interface/Interface.hpp"



namespace base {



class Client : public Interface
{
public:
   Client( const std::string&, const std::string& );
   ~Client( ) override;
};



} // namespace base
