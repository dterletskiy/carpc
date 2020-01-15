#pragma once

// Application
#include "imp/app/api/onoff/Data.hpp"
// Framework



namespace api::onoff {



class Server
   : public OnOffEvent::Consumer
{
public:
   Server( );
   ~Server( );

public:
   virtual void request_trigger_state( const std::string& ) = 0;
   void response_trigger_state( const bool );

private:
   void process_event( const OnOffEvent::Event& ) override;
};



} // namespace api::onoff
