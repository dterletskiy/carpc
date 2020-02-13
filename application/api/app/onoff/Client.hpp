#pragma once

// Framework
#include "api/sys/comm/interface/TProxy.hpp"
#include "api/sys/comm/interface/TClient.hpp"
// Application
#include "Data.hpp"



namespace api::onoff {



class Client
   : public base::TClient< data::Types >
{
public:
   Client( const std::string& );
   virtual ~Client( );

private:
   void connected( ) override;
   void disconnected( ) override;

private:
   void process_response_event( const data::OnOffEvent::Event& ) override;

public:
   const size_t request_trigger_state( const std::string&, const size_t );
   virtual void request_trigger_state_failed( ) = 0;
   virtual void response_trigger_state( const bool ) = 0;

private:
   base::TProxy< data::Types >* mp_proxy = nullptr;
};



} // namespace api::onoff
