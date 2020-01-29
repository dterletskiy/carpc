#pragma once

// Interface
#include "imp/app/api/onoff/Client.hpp"



namespace application::onoff {



class Client
   : public api::onoff::Client
{
public:
   Client( const std::string& );
   ~Client( );

public:
   void response_trigger_state( const bool ) override;
};



} // namespace application::onoff
