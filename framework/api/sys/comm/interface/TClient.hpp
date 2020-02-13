#pragma once

#include "api/sys/comm/interface/IInterface.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TClient"



namespace base {

   template< typename tID >
   class RequestCount
   {
   public:
      bool inc( const tID& );
      bool dec( const tID& );
      bool emplace( const tID& );

   private:
      std::map< tID, size_t > map;
   };

};



namespace base {



template< typename TYPES >
class TServer;



template< typename TYPES >
class TClient
   : public IInterface
   , public TYPES::tEventConsumer
{
   using tServer = TServer< TYPES >;

public:
   TClient( const std::string&, const std::string& );
   ~TClient( ) override;

private:
   void connected( const void* const ) override final;
   void disconnected( const void* const ) override final;
private:
   virtual void connected( ) = 0;
   virtual void disconnected( ) = 0;

private:
   const tServer* mp_server = nullptr;

protected:
   template< typename tRequestData, typename... Args >
      const SequenceID create_send_request( const Args&... args );
   template< typename tResponseData >
      const tResponseData* get_event_data( const typename TYPES::tEvent& event );

private:
   void process_event( const typename TYPES::tEvent& ) override final;
   virtual void process_response_event( const typename TYPES::tEvent& ) = 0;

private:
   bool inc_request_count( const typename TYPES::tEventID& );
   bool dec_request_count( const typename TYPES::tEventID& );
   std::map< typename TYPES::tEventID, size_t >    m_request_count_map;

private:
   SequenceID                                      m_seq_id = 0;
};



template< typename TYPES >
TClient< TYPES >::TClient( const std::string& name, const std::string& role_name )
   : IInterface( name, role_name )
   , TYPES::tEventConsumer( )
{
   for( auto rr_item : TYPES::RR )
   {
      m_request_count_map.emplace( std::make_pair( rr_item.request, 0 ) );
   }

   InterfaceEvent::Event::set_notification( this, { eInterface::ServerConnected } );
   InterfaceEvent::Event::create_send( { eInterface::ClientConnected }, { this }, TYPES::COMM_TYPE );
}

template< typename TYPES >
TClient< TYPES >::~TClient( )
{
   TYPES::tEvent::clear_all_notifications( this );
   InterfaceEvent::Event::clear_all_notifications( this );
   InterfaceEvent::Event::create_send( { eInterface::ClientDisconnected }, { this }, TYPES::COMM_TYPE );
}

template< typename TYPES >
void TClient< TYPES >::connected( const void* const p_server )
{
   InterfaceEvent::Event::clear_notification( this, { eInterface::ServerConnected } );
   InterfaceEvent::Event::set_notification( this, { eInterface::ServerDisconnected } );

   mp_server = reinterpret_cast< const tServer* >( p_server );
   connected( );
}

template< typename TYPES >
void TClient< TYPES >::disconnected( const void* const p_server )
{
   InterfaceEvent::Event::set_notification( this, { eInterface::ServerConnected } );
   InterfaceEvent::Event::clear_notification( this, { eInterface::ServerDisconnected } );

   mp_server = nullptr;
   disconnected( );
}

template< typename TYPES >
void TClient< TYPES >::process_event( const typename TYPES::tEvent& event )
{
   const typename TYPES::tEventID event_id = event.info( ).id( );
   const void* p_from_addr = event.info( ).from_addr( );
   const SequenceID seq_id = event.info( ).seq_id( );

   SYS_TRC( "processing event: %s", event.info( ).name( ).c_str( ) );

   typename TYPES::tEventID related_request_id = event_id;
   for( auto item : TYPES::RR )
   {
      if( item.response != event_id && item.busy != event_id )
         continue;

      related_request_id = item.request;
      break;
   }

   if( false == dec_request_count( related_request_id ) )
      return;

   process_response_event( event );
}

template< typename TYPES >
bool TClient< TYPES >::inc_request_count( const typename TYPES::tEventID& event_id )
{
   auto iterator = m_request_count_map.find( event_id );
   if( m_request_count_map.end( ) == iterator )
   {
      SYS_WRN( "request id does not exist: %s", to_string( event_id ).c_str( ) );
      return false;
   }

   size_t& count = iterator->second;
   if( 0 == count )
   {
      for( auto item : TYPES::RR )
      {
         if( item.request != event_id )
            continue;

         if( TYPES::tEventID::Undefined != item.response )
            TYPES::tEvent::set_notification( this, typename TYPES::tEvent::Signature( role( ), item.response, nullptr, this, 0 ) );
         if( TYPES::tEventID::Undefined != item.busy )
            TYPES::tEvent::set_notification( this, typename TYPES::tEvent::Signature( role( ), item.busy, nullptr, this, 0 ) );

         break;
      }
   }

   ++count;
   return true;
}

template< typename TYPES >
bool TClient< TYPES >::dec_request_count( const typename TYPES::tEventID& event_id )
{
   auto iterator = m_request_count_map.find( event_id );
   if( m_request_count_map.end( ) == iterator )
   {
      SYS_WRN( "request id does not exist: %s", to_string( event_id ).c_str( ) );
      return false;
   }

   size_t& count = --( m_request_count_map.at( event_id ) );
   if( 0 == count )
   {
      for( auto item : TYPES::RR )
      {
         if( item.request != event_id )
            continue;

         if( TYPES::tEventID::Undefined != item.response )
            TYPES::tEvent::clear_notification( this, typename TYPES::tEvent::Signature( role( ), item.response, nullptr, this, 0 ) );
         if( TYPES::tEventID::Undefined != item.busy )
            TYPES::tEvent::clear_notification( this, typename TYPES::tEvent::Signature( role( ), item.busy, nullptr, this, 0 ) );

         break;
      }
   }

   return true;
}

template< typename TYPES >
template< typename tRequestData, typename... Args >
const SequenceID TClient< TYPES >::create_send_request( const Args&... args )
{
   if( nullptr == mp_server )
      return InvalidSequenceID;

   if( false == inc_request_count( tRequestData::id ) )
      return InvalidSequenceID;

   typename TYPES::tEventData data( std::make_shared< tRequestData >( args... ) );
   TYPES::tEvent::create_send( typename TYPES::tEvent::Signature( role( ), tRequestData::id, this, mp_server, ++m_seq_id ), data, TYPES::COMM_TYPE );
   return m_seq_id;
}

template< typename TYPES >
template< typename tResponseData >
const tResponseData* TClient< TYPES >::get_event_data( const typename TYPES::tEvent& event )
{
   return static_cast< tResponseData* >( event.data( )->ptr.get( ) );
}



} // namespace base



#undef CLASS_ABBR
