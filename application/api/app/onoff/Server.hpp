#pragma once

// Framework
#include "api/sys/comm/interface/TServer.hpp"
// Application
#include "Data.hpp"



namespace api::onoff {



class Server
   : public base::TServer< data::Types >
{
public:
   Server( const std::string& );
   virtual ~Server( );

private:
   void connected( ) override;
   void disconnected( ) override;

private:
   void process_request_event( const data::OnOffEvent::Event& ) override;

public:
   virtual void request_trigger_state( const std::string&, const size_t ) = 0;
   void response_trigger_state( const bool );
};



} // namespace api::onoff
