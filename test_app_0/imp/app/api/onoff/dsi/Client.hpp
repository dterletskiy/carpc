#pragma once

// Application
#include "Data.hpp"
// Framework



namespace api::onoff::dsi {



class Client
   : public ServiceOnOff::OnOffEvent::Consumer
{
public:
   Client( );
   virtual ~Client( );

public:
   void request_trigger_state( const std::string& );
   virtual void response_trigger_state( const bool ) = 0;

private:
   void process_event( const ServiceOnOff::OnOffEvent::Event& ) override;
};



} // namespace api::onoff::dsi
