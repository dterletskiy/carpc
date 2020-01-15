#pragma once

// Interface
#include "imp/app/api/onoff/dsi/Server.hpp"



namespace application::onoff {



class Server
   : public api::onoff::dsi::Server
{
public:
   Server( );
   ~Server( );

public:
   void request_trigger_state( const std::string& ) override;
};



} // namespace application::onoff
