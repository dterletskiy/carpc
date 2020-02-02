#pragma once

// Framework
#include "api/sys/comm/interface/Client.hpp"
// Application
#include "Data.hpp"



namespace api::onoff {



class Client
   : public base::Client
   , public data::OnOffEvent::Consumer
{
public:
   Client( const std::string& );
   virtual ~Client( );

private:
   void connected( const base::Interface* const ) const override;
   void disconnected( const base::Interface* const ) const override;

public:
   void request_trigger_state( const std::string&, const size_t );
   virtual void response_trigger_state( const bool ) = 0;

private:
   void process_event( const data::OnOffEvent::Event& ) override;
};



} // namespace api::onoff
