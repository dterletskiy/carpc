#pragma once

// Application
#include "Data.hpp"



namespace api::onoff {



namespace data = api::onoff::ipc;



class Client
   : public data::OnOffEvent::Consumer
{
public:
   Client( const base::ServiceName&, const bool exported = true );
   virtual ~Client( );

public:
   void request_trigger_state( const std::string&, const size_t );
   virtual void response_trigger_state( const bool ) = 0;

private:
   void process_event( const data::OnOffEvent::Event& ) override;

private:
   base::ServiceName m_role_name;
   bool m_exported = false;
   base::eCommType m_comm_type = base::eCommType::ITC;

private:
   template< typename tResponseData, typename... Args >
      void create_send_event( const eOnOff, const Args&... );
   template< typename tRequestData >
      const tRequestData* get_event_data( const data::OnOffEvent::Event& );
};



template< typename tResponseData, typename... Args >
void Client::create_send_event( const eOnOff event_id, const Args&... args )
{
   data::OnOffEvent::Data data( std::make_shared< tResponseData >( args... ) );
   data::OnOffEvent::Event::create_send( m_role_name, event_id, data, m_comm_type );
}

template< typename tRequestData >
const tRequestData* Client::get_event_data( const data::OnOffEvent::Event& event )
{
   return static_cast< tRequestData* >( event.data( )->ptr.get( ) );
}



} // namespace api::onoff
