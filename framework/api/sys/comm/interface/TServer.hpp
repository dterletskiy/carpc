#pragma once

#include "api/sys/comm/interface/IInterface.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TServer"



namespace base {

   enum class eRequestStatus : size_t { BUSY, READY };

   struct RequestInfo
   {
      RequestInfo( const SequenceID _server_seq_id )
         : server_seq_id( _server_seq_id )
      { }
      RequestInfo( const SequenceID _server_seq_id, const SequenceID _client_seq_id, const void* _client_addr )
         : server_seq_id( _server_seq_id )
         , client_seq_id( _client_seq_id )
         , client_addr( _client_addr )
      { }
      const bool operator<( const RequestInfo& other ) const { return server_seq_id < other.server_seq_id; }

      SequenceID server_seq_id = InvalidSequenceID;
      SequenceID client_seq_id = InvalidSequenceID;
      const void* client_addr = nullptr;
   };

   struct RequestStatus
   {
      eRequestStatus status = eRequestStatus::READY;
      SequenceID processing_server_seq_id = InvalidSequenceID;
      std::set< RequestInfo > info_set;
   };

}



namespace base {



template< typename TYPES >
   class TProxy;



template< typename TYPES >
class TServer
   : public IInterface
   , public TYPES::tEventConsumer
{
protected:
   using tProxy = TProxy< TYPES >;
   using tRequestResponseIDs = RequestResponseIDs< typename TYPES::tEventID >;
   struct Comparator { bool operator( )( const tRequestResponseIDs& rr1, const tRequestResponseIDs& rr2 ) const { return rr1.request < rr2.request; } };
   using tRequestStatusMap = std::map< tRequestResponseIDs, RequestStatus, Comparator >;

public:
   TServer( const std::string&, const std::string& );
   ~TServer( ) override;

private:
   void connected( const void* const ) override final;
   void disconnected( const void* const ) override final;
private:
   virtual void connected( ) = 0;
   virtual void disconnected( ) = 0;

private:
   std::set< const tProxy* > mp_proxy_set;

protected:
   template< typename tResponseData, typename... Args >
      void response( const Args&... args );
   template< typename tNotificationData, typename... Args >
      void notify( const Args&... args );
   template< typename tNotificationData >
      const tNotificationData* attribute( ) const;
   template< typename tRequestData >
      const tRequestData* get_event_data( const typename TYPES::tEvent& event );

private:
   void process_event( const typename TYPES::tEvent& ) override final;
   virtual void process_request_event( const typename TYPES::tEvent& ) = 0;
private:
   bool prepare_request( const typename TYPES::tEvent& );
   template< typename tResponseData >
      std::optional< RequestInfo > prepare_response( );
public:
   const SequenceID unblock_request( );
   void prepare_response( const SequenceID );

protected:
   tRequestStatusMap                         m_request_status_map;
   SequenceID                                m_seq_id = 0;
   std::optional< typename TYPES::tEventID > m_processing_event_id = std::nullopt;
   std::optional< SequenceID >               m_processing_seq_id = std::nullopt;
   std::map< typename TYPES::tEventID, std::shared_ptr< typename TYPES::tBaseData > > m_attribute_map;
};




template< typename TYPES >
TServer< TYPES >::TServer( const std::string& name, const std::string& role_name )
   : IInterface( name, role_name )
   , TYPES::tEventConsumer( )
{
   for( auto rr_item : TYPES::RR )
   {
      m_request_status_map.emplace( rr_item, RequestStatus{ } );
      TYPES::tEvent::set_notification( this, typename TYPES::tSignature( role( ), rr_item.request, nullptr, nullptr, 0 ) );
   }
   for( auto n_item : TYPES::N )
   {
      m_attribute_map.emplace( n_item.notification, nullptr );
   }

   InterfaceEvent::Event::set_notification( this, { role( ), eInterface::ClientConnected } );
   InterfaceEvent::Event::set_notification( this, { role( ), eInterface::ClientDisconnected } );
   InterfaceEvent::Event::create_send( { role( ), eInterface::ServerConnected }, { this }, TYPES::COMM_TYPE );
}

template< typename TYPES >
TServer< TYPES >::~TServer( )
{
   TYPES::tEvent::clear_all_notifications( this );
   InterfaceEvent::Event::clear_all_notifications( this );
   InterfaceEvent::Event::create_send( { role( ), eInterface::ServerDisconnected }, { this }, TYPES::COMM_TYPE );
}

template< typename TYPES >
void TServer< TYPES >::connected( const void* const p_proxy )
{
   // Check if current proxy was connected previously
   const tProxy* const _p_proxy = reinterpret_cast< const tProxy* const >( p_proxy );
   auto iterator = mp_proxy_set.find( _p_proxy );
   if( mp_proxy_set.end( ) != iterator )
      return;

   // Add connected proxy to local collection
   mp_proxy_set.emplace( _p_proxy );
   // Notifying final server implementation about connected proxy
   connected( );

   // Send event to all consumers (just connected proxy) that server is connected
   InterfaceEvent::Event::create_send( { role( ), eInterface::ServerConnected }, { this }, TYPES::COMM_TYPE );
}

template< typename TYPES >
void TServer< TYPES >::disconnected( const void* const p_proxy )
{
   // Remove just disconnected proxy from local collection
   const tProxy* const _p_proxy = reinterpret_cast< const tProxy* const >( p_proxy );
   mp_proxy_set.erase( _p_proxy );
   // Notifying final server implementation about disconnected proxy
   disconnected( );
}

template< typename TYPES >
void TServer< TYPES >::process_event( const typename TYPES::tEvent& event )
{
   SYS_TRC( "processing event: %s", event.info( ).name( ).c_str( ) );
   m_processing_event_id = event.info( ).id( );

   if( true == prepare_request( event ) )
   {
      process_request_event( event );
   }
   else
   {
      SYS_WRN( "not processable event: %s", event.info( ).name( ).c_str( ) )
   }

   m_processing_event_id.reset( );
}

template< typename TYPES >
bool TServer< TYPES >::prepare_request( const typename TYPES::tEvent& event )
{
   const typename TYPES::tEventID event_id = event.info( ).id( );
   const void* p_from_addr = event.info( ).from_addr( );
   const SequenceID seq_id = event.info( ).seq_id( );

   // Find request event_id in request status map
   auto iterator_status_map = m_request_status_map.find( tRequestResponseIDs( event_id ) );
   if( m_request_status_map.end( ) == iterator_status_map )
   {
      SYS_WRN( "not a request ID: %s", to_string( event_id ).c_str( ) );
      return false;
   }
   auto& rrIDs = iterator_status_map->first;
   auto& request_status = iterator_status_map->second;

   // Check if received request has connected response.
   // If current request does not have connected response request status should not be set to busy and all data for response is not needed.
   if( TYPES::tEventID::Undefined == rrIDs.response )
      return true;

   // Check request status for current request ID
   if( eRequestStatus::BUSY == request_status.status )
   {
      SYS_WRN( "request busy: %s", to_string( event_id ).c_str( ) );
      // Sending event with request busy id
      TYPES::tEvent::create_send( typename TYPES::tSignature( role( ), rrIDs.busy, this, p_from_addr, seq_id ), TYPES::COMM_TYPE );
      return false;
   }

   // Set request busy for current request ID
   request_status.status = eRequestStatus::BUSY;
   // Increment common sequence ID and set it's value to current processing sequence ID for current request ID
   request_status.processing_server_seq_id = ++m_seq_id;
   // Store RequestInfo structure to request info set 
   request_status.info_set.emplace( m_seq_id, seq_id, p_from_addr );
   return true;
}

template< typename TYPES >
template< typename tResponseData >
std::optional< RequestInfo > TServer< TYPES >::prepare_response( )
{
   // Find request id in request status map
   auto iterator_status_map = m_request_status_map.find( tRequestResponseIDs( tResponseData::REQUEST ) );
   if( m_request_status_map.end( ) == iterator_status_map )
   {
      SYS_WRN( "not a request ID: %s", to_string( tResponseData::REQUEST ).c_str( ) );
      return std::nullopt;
   }
   auto& request_status = iterator_status_map->second;
   auto& request_info_set = request_status.info_set;

   SequenceID serching_seq_id = m_processing_seq_id.value_or( request_status.processing_server_seq_id );
   // Search for RequestInfo structure in request infor set for current request ID
   auto iterator_info_set = request_info_set.find( RequestInfo{ serching_seq_id } );
   if( request_info_set.end( ) == iterator_info_set )
   {
      SYS_WRN( "request info for currnet sequence id has not been found: %zu", serching_seq_id );
      return std::nullopt;
   }

   RequestInfo request_info = *iterator_info_set;

   request_info_set.erase( iterator_info_set );
   request_status.status = eRequestStatus::READY;
   m_processing_seq_id.reset( );

   return request_info;
}

template< typename TYPES >
const SequenceID TServer< TYPES >::unblock_request( )
{
   if( std::nullopt == m_processing_event_id )
   {
      SYS_WRN( "unable unblock request" );
      return InvalidSequenceID;
   }

   // Find request id in request status map
   auto iterator_status_map = m_request_status_map.find( tRequestResponseIDs( m_processing_event_id.value( ) ) );
   if( m_request_status_map.end( ) == iterator_status_map )
   {
      SYS_WRN( "not a request ID: %s", to_string( m_processing_event_id.value( ) ).c_str( ) );
      return InvalidSequenceID;
   }
   auto& request_status = iterator_status_map->second;

   request_status.status = eRequestStatus::READY;
   return request_status.processing_server_seq_id;
}

template< typename TYPES >
void TServer< TYPES >::prepare_response( const SequenceID seq_id )
{
   m_processing_seq_id = seq_id;
}

template< typename TYPES >
template< typename tResponseData, typename... Args >
void TServer< TYPES >::response( const Args&... args )
{
   std::optional< RequestInfo > request_info = prepare_response< tResponseData >( );
   if( std::nullopt == request_info )
      return;

   typename TYPES::tEventData data( std::make_shared< tResponseData >( args... ) );
   TYPES::tEvent::create_send(
      typename TYPES::tSignature( role( ), tResponseData::RESPONSE, this, request_info.value( ).client_addr, request_info.value( ).client_seq_id )
      , data, TYPES::COMM_TYPE
   );
}

template< typename TYPES >
template< typename tNotificationData, typename... Args >
void TServer< TYPES >::notify( const Args&... args )
{
   auto iterator = m_attribute_map.find( tNotificationData::NOTIFICATION );
   if( m_attribute_map.end( ) == iterator )
   {
      SYS_WRN( "not a notification ID: %s", to_string( tNotificationData::NOTIFICATION ).c_str( ) );
      return;
   }

   std::shared_ptr< typename TYPES::tBaseData > p_base_data = std::make_shared< tNotificationData >( args... );
   typename TYPES::tEventData data( p_base_data );
   TYPES::tEvent::create_send(
      typename TYPES::tSignature( role( ), tNotificationData::NOTIFICATION, this, nullptr, 0 )
      , data, TYPES::COMM_TYPE
   );

   iterator->second = p_base_data;
}

template< typename TYPES >
template< typename tNotificationData >
const tNotificationData* TServer< TYPES >::attribute( ) const
{
   auto iterator = m_attribute_map.find( tNotificationData::NOTIFICATION );
   if( m_attribute_map.end( ) == iterator )
   {
      SYS_WRN( "not a notification ID: %s", to_string( tNotificationData::NOTIFICATION ).c_str( ) );
      return nullptr;
   }

   return static_cast< tNotificationData* >( iterator->second.get( ) );
}

template< typename TYPES >
template< typename tRequestData >
const tRequestData* TServer< TYPES >::get_event_data( const typename TYPES::tEvent& event )
{
   return static_cast< tRequestData* >( event.data( )->ptr.get( ) );
}



} // namespace base



#undef CLASS_ABBR
