#pragma once

// Interface
#include "imp/app/api/onoff/dsi/Client.hpp"



namespace application::onoff {



class Client
   : public api::onoff::dsi::Client
{
public:
   Client( );
   ~Client( );

public:
   void response_trigger_state( const bool ) override;
};



} // namespace application::onoff
