#pragma once

// Application
#include "Data.hpp"



namespace api::onoff::no_dsi {



class Client
   : public OnOffEvent::Consumer
{
public:
   Client( );
   virtual ~Client( );

public:
   void request_trigger_state( const std::string& );
   virtual void response_trigger_state( const bool ) = 0;

private:
   void process_event( const OnOffEvent::Event& ) override;
};



} // namespace api::onoff::no_dsi
