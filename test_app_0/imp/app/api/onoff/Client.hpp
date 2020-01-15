#pragma once

// Application
#include "imp/app/api/onoff/Data.hpp"
// Framework



namespace api::onoff {



class Client
   : public OnOffEvent::Consumer
{
public:
   Client( );
   ~Client( );

public:
   void request_trigger_state( const std::string& );
   virtual void response_trigger_state( const bool ) = 0;

private:
   void process_event( const OnOffEvent::Event& ) override;
};



} // namespace api::onoff
