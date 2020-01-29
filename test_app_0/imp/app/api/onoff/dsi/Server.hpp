#pragma once

// Application
#include "Data.hpp"



namespace api::onoff::dsi {



class Server
   : public OnOffEvent::Consumer
{
public:
   Server( const base::ServiceName& );
   virtual ~Server( );

public:
   virtual void request_trigger_state( const std::string&, const size_t ) = 0;
   void response_trigger_state( const bool );

private:
   void process_event( const OnOffEvent::Event& ) override;

private:
   base::ServiceName m_service_name;
};



} // namespace api::onoff::dsi
