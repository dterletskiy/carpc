#pragma once

// Application
#include "Data.hpp"



namespace api::onoff::no_dsi {



class Server
   : public OnOffEvent::Consumer
{
public:
   Server( );
   virtual ~Server( );

public:
   virtual void request_trigger_state( const std::string& ) = 0;
   void response_trigger_state( const bool );

private:
   void process_event( const OnOffEvent::Event& ) override;
};



} // namespace api::onoff::no_dsi
