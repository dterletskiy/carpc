#pragma once

// Interface
#include "api/app/onoff/Client.hpp"



namespace application::components::onoff {



class Client
   : public api::onoff::Client
{
public:
   Client( const std::string&, const std::string& name = { } );
   ~Client( );

private:
   const std::string m_name = { };

private:
   void response_trigger_state( const bool ) override;
   void request_trigger_state_failed( ) override;
   void on_current_state( const std::string& ) override;

public:
   void request_trigger_state( const std::string&, const size_t );
};



} // namespace application::components::onoff
