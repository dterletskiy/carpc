#pragma once

// Framework
#include "api/sys/comm/interface/Server.hpp"
// Application
#include "Data.hpp"



namespace api::onoff {



class Server
   : public base::Server
   , public data::OnOffEvent::Consumer
{
public:
   Server( const std::string& );
   virtual ~Server( );

private:
   void connected( const base::Interface* const ) const override;
   void disconnected( const base::Interface* const ) const override;

public:
   virtual void request_trigger_state( const std::string&, const size_t ) = 0;
   void response_trigger_state( const bool );

private:
   void process_event( const data::OnOffEvent::Event& ) override;
};



} // namespace api::onoff
