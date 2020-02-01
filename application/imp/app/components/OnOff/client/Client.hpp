#pragma once

// Interface
#include "api/app/onoff/Client.hpp"



namespace application::components {



class Client
   : public api::onoff::Client
{
public:
   Client( const std::string& );
   ~Client( );

public:
   void response_trigger_state( const bool ) override;
};



} // namespace application::components
