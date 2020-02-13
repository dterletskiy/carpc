#pragma once

#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/comm/interface/IInterface.hpp"
#include "api/sys/comm/interface/TClient.hpp"
#include "api/sys/service/ServiceThread.hpp"
#include "api/sys/service/ServiceProcess.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TProxy"



namespace base {



template< typename TYPES >
   class TServer;
template< typename TYPES >
   class TClient;



template< typename TYPES >
class TProxy
   : public IInterface
   , public TYPES::tEventConsumer
{
   using tServer = TServer< TYPES >;
   using tClient = TClient< TYPES >;
   using tProxy = TProxy< TYPES >;

private:
   TProxy( const std::string&, const std::string& );
   static std::map< TID, tProxy* > s_proxy_map;
public:
   ~TProxy( ) override;
   static tProxy* create( const std::string&, const std::string& );

private:
   void connected( const void* const ) override final;
   void disconnected( const void* const ) override final;

private:
   void process_event( const typename TYPES::tEvent& ) override final;

public:
   template< typename tRequestData, typename... Args >
      const SequenceID create_send_request( tClient*, const Args&... args );
   template< typename tResponseData >
      const tResponseData* get_event_data( const typename TYPES::tEvent& event );

private:
   bool inc_request_count( const typename TYPES::tEventID& );
   bool dec_request_count( const typename TYPES::tEventID& );
   std::map< typename TYPES::tEventID, size_t >    m_request_count_map;

public:
   void register_client( tClient* );
   void unregister_client( tClient* );
private:
   const tServer*                                  mp_server = nullptr;
   std::set< tClient* >                            m_client_set;
   SequenceID                                      m_seq_id = 0;
};



template< typename TYPES >
TProxy< TYPES >::TProxy( const std::string& name, const std::string& role_name )
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
TProxy< TYPES >::~TProxy( )
{
   TYPES::tEvent::clear_all_notifications( this );
   InterfaceEvent::Event::clear_all_notifications( this );
   InterfaceEvent::Event::create_send( { eInterface::ClientDisconnected }, { this }, TYPES::COMM_TYPE );
}

template< typename TYPES >
std::map< TID, TProxy< TYPES >* > TProxy< TYPES >::s_proxy_map;

template< typename TYPES >
TProxy< TYPES >* TProxy< TYPES >::create( const std::string& name, const std::string& role_name )
{
   TID tid = ServiceProcess::instance( )->current_service( )->id( );
   os::Mutex mutex( true );

   auto iterator = s_proxy_map.find( tid );
   if( s_proxy_map.end( ) != iterator )
   {
      SYS_TRC( "return existing proxy: %p", iterator->second );
      return iterator->second;
   }

   tProxy* p_proxy = new tProxy( name, role_name );
   if( nullptr == p_proxy )
   {
      SYS_ERR( "unable create proxy" );
      return nullptr;
   }
   auto pair = s_proxy_map.emplace( tid, p_proxy );
   if( false == pair.second )
   {
      SYS_ERR( "unable emplace create proxy" );
      delete p_proxy;
   }
   SYS_TRC( "proxy created: %p", p_proxy );

   return p_proxy;
}

template< typename TYPES >
void TProxy< TYPES >::register_client( tClient* p_client )
{
   if( nullptr == p_client )
      return;

   m_client_set.emplace( p_client );
   if( nullptr != mp_server )
      p_client->connected( );
}

template< typename TYPES >
void TProxy< TYPES >::unregister_client( tClient* p_client )
{
   if( nullptr == p_client )
      return;

   m_client_set.erase( p_client );
}


template< typename TYPES >
void TProxy< TYPES >::connected( const void* const p_server )
{
   InterfaceEvent::Event::clear_notification( this, { eInterface::ServerConnected } );
   InterfaceEvent::Event::set_notification( this, { eInterface::ServerDisconnected } );

   mp_server = reinterpret_cast< const tServer* >( p_server );
   for( auto item : m_client_set )
      item->connected( );
}

template< typename TYPES >
void TProxy< TYPES >::disconnected( const void* const p_server )
{
   InterfaceEvent::Event::set_notification( this, { eInterface::ServerConnected } );
   InterfaceEvent::Event::clear_notification( this, { eInterface::ServerDisconnected } );

   mp_server = nullptr;
   for( auto item : m_client_set )
      item->disconnected( );
}

template< typename TYPES >
void TProxy< TYPES >::process_event( const typename TYPES::tEvent& event )
{
   const typename TYPES::tEventID event_id = event.info( ).id( );
   const void* p_from_addr = event.info( ).from_addr( );
   const void* p_to_addr = event.info( ).to_addr( );
   const SequenceID seq_id = event.info( ).seq_id( );

   SYS_TRC( "processing event: %s", event.info( ).name( ).c_str( ) );

   typename TYPES::tEventID related_request_id = event_id;
   for( auto item : TYPES::RR )
   {
      if( item.response == event_id || item.busy == event_id )
      {
         related_request_id = item.request;
         break;
      }
   }

   if( false == dec_request_count( related_request_id ) )
      return;

   auto iterator = m_client_set.find( (tClient*)p_to_addr );
   if( m_client_set.end( ) == iterator )
   {
      SYS_WRN( "delivered event to unknown client: %p", p_to_addr )
      return;
   }

   (*iterator)->process_response_event( event );
}

template< typename TYPES >
bool TProxy< TYPES >::inc_request_count( const typename TYPES::tEventID& event_id )
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
            TYPES::tEvent::set_notification( this, typename TYPES::tEvent::Signature( role( ), item.response, nullptr, nullptr, 0 ) );
         if( TYPES::tEventID::Undefined != item.busy )
            TYPES::tEvent::set_notification( this, typename TYPES::tEvent::Signature( role( ), item.busy, nullptr, nullptr, 0 ) );

         break;
      }
   }

   ++count;
   return true;
}

template< typename TYPES >
bool TProxy< TYPES >::dec_request_count( const typename TYPES::tEventID& event_id )
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
            TYPES::tEvent::clear_notification( this, typename TYPES::tEvent::Signature( role( ), item.response, nullptr, nullptr, 0 ) );
         if( TYPES::tEventID::Undefined != item.busy )
            TYPES::tEvent::clear_notification( this, typename TYPES::tEvent::Signature( role( ), item.busy, nullptr, nullptr, 0 ) );

         break;
      }
   }

   return true;
}

template< typename TYPES >
template< typename tRequestData, typename... Args >
const SequenceID TProxy< TYPES >::create_send_request( tClient* p_client, const Args&... args )
{
   if( nullptr == mp_server )
      return InvalidSequenceID;

   if( false == inc_request_count( tRequestData::id ) )
      return InvalidSequenceID;

   typename TYPES::tEventData data( std::make_shared< tRequestData >( args... ) );
   TYPES::tEvent::create_send( typename TYPES::tEvent::Signature( role( ), tRequestData::id, p_client, mp_server, ++m_seq_id ), data, TYPES::COMM_TYPE );

   return m_seq_id;
}

template< typename TYPES >
template< typename tResponseData >
const tResponseData* TProxy< TYPES >::get_event_data( const typename TYPES::tEvent& event )
{
   return static_cast< tResponseData* >( event.data( )->ptr.get( ) );
}



} // namespace base



#undef CLASS_ABBR
