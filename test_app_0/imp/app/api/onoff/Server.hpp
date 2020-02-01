#pragma once

// Framework
#include "api/sys/comm/interface/Server.hpp"
// Application
#include "Data.hpp"



namespace api::onoff {



namespace data = api::onoff::ipc;



class Server
   : public base::Server
   , public data::OnOffEvent::Consumer
{
public:
   Server( const std::string&, const bool exported = true );
   virtual ~Server( );

private:
   void connected( const base::Interface* const ) const override;
   void disconnected( const base::Interface* const ) const override;

public:
   virtual void request_trigger_state( const std::string&, const size_t ) = 0;
   void response_trigger_state( const bool );

private:
   void process_event( const data::OnOffEvent::Event& ) override;

private:
   bool m_exported = false;
   base::eCommType m_comm_type = base::eCommType::ITC;

private:
   template< typename tResponseData, typename... Args >
      void create_send_event( const eOnOff, const Args&... );
   template< typename tRequestData >
      const tRequestData* get_event_data( const data::OnOffEvent::Event& );
};



template< typename tResponseData, typename... Args >
void Server::create_send_event( const eOnOff event_id, const Args&... args )
{
   data::OnOffEvent::Data data( std::make_shared< tResponseData >( args... ) );
   data::OnOffEvent::Event::create_send( role( ), event_id, data, m_comm_type );
}

template< typename tRequestData >
const tRequestData* Server::get_event_data( const data::OnOffEvent::Event& event )
{
   return static_cast< tRequestData* >( event.data( )->ptr.get( ) );
}



} // namespace api::onoff
