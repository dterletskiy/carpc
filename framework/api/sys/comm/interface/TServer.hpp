#pragma once

#include "api/sys/comm/interface/IServer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TServer"



namespace base::interface {

   template< typename TYPES >
      class TProxy;

}



namespace base::interface {

   enum class eRequestStatus : size_t { BUSY, READY };

   struct RequestInfo
   {
      RequestInfo( const tSequenceID _server_seq_id )
         : server_seq_id( _server_seq_id )
      { }
      RequestInfo( const tSequenceID _server_seq_id, const tSequenceID _client_seq_id, const Address _client_addr )
         : server_seq_id( _server_seq_id )
         , client_seq_id( _client_seq_id )
         , client_addr( _client_addr )
      { }
      const bool operator<( const RequestInfo& other ) const { return server_seq_id < other.server_seq_id; }

      tSequenceID server_seq_id = InvalidSequenceID;
      tSequenceID client_seq_id = InvalidSequenceID;
      const Address client_addr;
   };

   struct RequestStatus
   {
      eRequestStatus status = eRequestStatus::READY;
      tSequenceID processing_server_seq_id = InvalidSequenceID;
      std::set< RequestInfo > info_set;
   };

}



namespace base::interface {

   template< typename TYPES >
   struct NotificationStatus
   {
      bool is_subscribed = false;
      std::shared_ptr< typename TYPES::tBaseData > data = nullptr;
   };

}



namespace base::interface {

   template< typename TYPES >
   class TServer
      : public IServer
      , public TYPES::tEventConsumer
   {
      using tRequestResponseIDs = RequestResponseIDs< typename TYPES::tEventID >;
      using tRequestStatusMap = std::map< tRequestResponseIDs, RequestStatus, typename tRequestResponseIDs::Comparator::ByRequest >;
      using tNotificationStatus = NotificationStatus< TYPES >;
      using tAttributeStatusMap = std::map< typename TYPES::tEventID, tNotificationStatus >;

      public:
         TServer( const tAsyncTypeID&, const std::string&, const bool );
         ~TServer( ) override;

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
      private:
         virtual void process_request_event( const typename TYPES::tEvent& ) = 0;
         bool prepare_request( const typename TYPES::tEvent& );
         template< typename tResponseData >
            std::optional< RequestInfo > prepare_response( );
         bool process_subscription( const typename TYPES::tEvent& );
      public:
         const tSequenceID unblock_request( );
         void prepare_response( const tSequenceID );

      private:
         tRequestStatusMap                         m_request_status_map;
         tSequenceID                               m_seq_id = 0;
         std::optional< typename TYPES::tEventID > m_processing_event_id = std::nullopt;
         std::optional< tSequenceID >              m_processing_seq_id = std::nullopt;
         tAttributeStatusMap                       m_attribute_status_map;
   };




   template< typename TYPES >
   TServer< TYPES >::TServer( const tAsyncTypeID& interface_type_id, const std::string& role_name, const bool is_export )
      : IServer( interface_type_id, role_name, is_export )
      , TYPES::tEventConsumer( )
   {
      for( auto rr_item : TYPES::RR )
      {
         m_request_status_map.emplace( rr_item, RequestStatus{ } );
         TYPES::tEvent::set_notification(
            this,
            typename TYPES::tSignature( signature( ).role( ), rr_item.request, Address{ nullptr }, Address{ this } )
         );
      }
      for( auto n_item : TYPES::N )
      {
         m_attribute_status_map.emplace( n_item.notification, tNotificationStatus{ } );
         TYPES::tEvent::set_notification(
            this,
            typename TYPES::tSignature( signature( ).role( ), n_item.subscribe, Address{ nullptr }, Address{ this } )
         );
         TYPES::tEvent::set_notification(
            this,
            typename TYPES::tSignature( signature( ).role( ), n_item.unsubscribe, Address{ nullptr }, Address{ this } )
         );
      }
   }

   template< typename TYPES >
   TServer< TYPES >::~TServer( )
   {
      TYPES::tEvent::clear_all_notifications( this );
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
      else if( true == process_subscription( event ) )
      {
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
      const Address& from_addr = event.info( ).from_addr( );
      const tSequenceID seq_id = event.info( ).seq_id( );

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
         TYPES::tEvent::create_send(
            typename TYPES::tSignature( signature( ).role( ), rrIDs.busy, Address{ this }, from_addr, seq_id ), TYPES::COMM_TYPE
         );
         return false;
      }

      // Set request busy for current request ID
      request_status.status = eRequestStatus::BUSY;
      // Increment common sequence ID and set it's value to current processing sequence ID for current request ID
      request_status.processing_server_seq_id = ++m_seq_id;
      // Store RequestInfo structure to request info set 
      request_status.info_set.emplace( m_seq_id, seq_id, from_addr );
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

      tSequenceID serching_seq_id = m_processing_seq_id.value_or( request_status.processing_server_seq_id );
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
   const tSequenceID TServer< TYPES >::unblock_request( )
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
   void TServer< TYPES >::prepare_response( const tSequenceID seq_id )
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

      typename TYPES::tSignature event_signature(
         signature( ).role( ), tResponseData::RESPONSE, this, request_info.value( ).client_addr, request_info.value( ).client_seq_id
      );
      typename TYPES::tEventData data( std::make_shared< tResponseData >( args... ) );
      TYPES::tEvent::create_send( event_signature, data, TYPES::COMM_TYPE );
   }

   template< typename TYPES >
   bool TServer< TYPES >::process_subscription( const typename TYPES::tEvent& event )
   {
      const typename TYPES::tEventID event_id = event.info( ).id( );
      const Address& from_addr = event.info( ).from_addr( );

      for( auto& item : TYPES::N )
      {
         if( item.subscribe != event_id && item.unsubscribe != event_id )
            continue;

         auto iterator = m_attribute_status_map.find( item.notification );
         if( m_attribute_status_map.end( ) == iterator )
         {
            SYS_WRN( "was not found notification ID: %s", to_string( item.notification ).c_str( ) );
            return false;
         }

         tNotificationStatus& notification_status = iterator->second;

         SYS_TRC( "notification subscribe/unsubscribe ID: %s", to_string( event_id ).c_str( ) );
         notification_status.is_subscribed = item.subscribe == event_id;

         if( notification_status.is_subscribed && notification_status.data )
         {
            typename TYPES::tSignature event_signature( signature( ).role( ), item.notification, Address{ this }, from_addr );
            typename TYPES::tEventData data( notification_status.data );
            TYPES::tEvent::create_send( event_signature, data, TYPES::COMM_TYPE );
         }

         return true;
      }

      SYS_WRN( "not a notification subscribe/unsubscribe ID: %s", to_string( event_id ).c_str( ) );
      return false;
   }

   template< typename TYPES >
   template< typename tNotificationData, typename... Args >
   void TServer< TYPES >::notify( const Args&... args )
   {
      auto iterator = m_attribute_status_map.find( tNotificationData::NOTIFICATION );
      if( m_attribute_status_map.end( ) == iterator )
      {
         SYS_WRN( "not a notification ID: %s", to_string( tNotificationData::NOTIFICATION ).c_str( ) );
         return;
      }

      std::shared_ptr< typename TYPES::tBaseData > p_base_data = std::make_shared< tNotificationData >( args... );
      iterator->second.data = p_base_data;

      if( true == iterator->second.is_subscribed )
      {
         typename TYPES::tSignature event_signature(
            signature( ).role( ), tNotificationData::NOTIFICATION, Address{ this }, Address{ nullptr }
         );
         typename TYPES::tEventData data( p_base_data );
         TYPES::tEvent::create_send( event_signature, data, TYPES::COMM_TYPE );
      }
   }

   template< typename TYPES >
   template< typename tNotificationData >
   const tNotificationData* TServer< TYPES >::attribute( ) const
   {
      auto iterator = m_attribute_status_map.find( tNotificationData::NOTIFICATION );
      if( m_attribute_status_map.end( ) == iterator )
      {
         SYS_WRN( "not a notification ID: %s", to_string( tNotificationData::NOTIFICATION ).c_str( ) );
         return nullptr;
      }

      return static_cast< tNotificationData* >( iterator->second.data.get( ) );
   }

   template< typename TYPES >
   template< typename tRequestData >
   const tRequestData* TServer< TYPES >::get_event_data( const typename TYPES::tEvent& event )
   {
      return static_cast< tRequestData* >( event.data( )->ptr.get( ) );
   }

} // namespace base::interface



#undef CLASS_ABBR
