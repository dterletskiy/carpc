#pragma once

// Application
#include "Data.hpp"



namespace api::onoff::no_dsi {



class Client
   : public OnOffEvent::Consumer
{
public:
   Client( const base::ServiceName& );
   virtual ~Client( );

public:
   void request_trigger_state( const std::string&, const size_t );
   virtual void response_trigger_state( const bool ) = 0;

private:
   void process_event( const OnOffEvent::Event& ) override;

private:
   base::ServiceName m_service_name;
};



} // namespace api::onoff::no_dsi
