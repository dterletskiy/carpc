#pragma once

#include "api/sys/application/Process.hpp"
#include "api/sys/comm/service/IServer.hpp"
#include "api/sys/comm/service/experimental/TGenerator.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TServerFast"



namespace carpc::service::experimental::__private__ {

   template< typename TYPES >
      class TProxy;

   template< typename TYPES >
      class TServer;

} // namespace carpc::service::experimental::__private__



namespace carpc::service::experimental::__private_server__ {

   enum class eRequestStatus : std::uint8_t { BUSY, READY };

   struct RequestInfo
   {
      RequestInfo( const comm::sequence::ID _server_seq_id )
         : server_seq_id( _server_seq_id )
      { }
      RequestInfo( const comm::sequence::ID _server_seq_id, const comm::sequence::ID _client_seq_id, const Address _client_addr )
         : server_seq_id( _server_seq_id )
         , client_seq_id( _client_seq_id )
         , client_addr( _client_addr )
      { }
      const bool operator<( const RequestInfo& other ) const { return server_seq_id < other.server_seq_id; }

      comm::sequence::ID server_seq_id = comm::sequence::ID::invalid;
      comm::sequence::ID client_seq_id = comm::sequence::ID::invalid;
      const Address client_addr;
   };

   struct RequestStatus
   {
      eRequestStatus status = eRequestStatus::READY;
      comm::sequence::ID processing_server_seq_id = comm::sequence::ID::invalid;
      std::set< RequestInfo > info_set;
   };

   template< typename TYPES >
   class MethodProcessor
   {
      using tServer = __private__::TServer< TYPES >;
      using tProxy = __private__::TProxy< TYPES >;

      public:
         using tRequestStatusMap = std::map< typename TYPES::method::tEventID, RequestStatus >;

      public:
         MethodProcessor( tServer* );
         ~MethodProcessor( );

         void process( const typename TYPES::method::tEvent& );
         const comm::sequence::ID& unblock_request( );
         void prepare_response( const comm::sequence::ID );
         template< typename tResponseData, typename... Args >
            void response( const Args&... args );

      private:
         bool pre_process( const typename TYPES::method::tEvent& );

      private:
         tServer*                                           mp_server = nullptr;
         tRequestStatusMap                                  m_request_status_map;
         comm::sequence::ID                                 m_seq_id = comm::sequence::ID::zero;
         std::optional< typename TYPES::method::tEventID >  m_processing_event_id = std::nullopt;
         std::optional< comm::sequence::ID >                m_prepare_response_seq_id = std::nullopt;
   };

   template< typename TYPES >
   MethodProcessor< TYPES >::MethodProcessor( tServer* p_server )
      : mp_server( p_server )
   {
   }

   template< typename TYPES >
   MethodProcessor< TYPES >::~MethodProcessor( )
   {
   }

   template< typename TYPES >
   bool MethodProcessor< TYPES >::pre_process( const typename TYPES::method::tEvent& event )
   {
      const typename TYPES::method::tEventID event_id = event.info( ).id( );
      const comm::service::ID from_id = event.info( ).from( );
      const comm::service::ID to_id = event.info( ).to( );
      const comm::sequence::ID seq_id = event.info( ).seq_id( );
      const auto from_context = event.context( );

      // @TDA-DEBUG:
      if( to_id != mp_server->id( ) )
      {
         SYS_ERR( "%s != %s", to_id.name( ).c_str( ), mp_server->id( ).name( ).c_str( ) );
      }

      auto result = m_request_status_map.emplace( event_id, RequestStatus{ } );
      auto& request_status = result.first->second;

      // Check if received request has connected response.
      // If current request does not have connected response request status should not be set to busy
      // and all data for response is not needed.
      if( false == TYPES::T::method::has_response( event_id ) )
         return true;

      // Check request status for current request ID
      if( eRequestStatus::BUSY == request_status.status )
      {
         SYS_WRN( "request busy: %s", event_id.c_str( ) );
         // Sending event with request busy id
         TYPES::method::tEvent::create_send(
            typename TYPES::method::tEventUserSignature(
               mp_server->signature( ).role( ),
               event_id,
               carpc::service::eType::BUSY,
               mp_server->id( ),
               from_id,
               seq_id
            ),
            from_context
         );
         return false;
      }

      // Set request busy for current request ID
      request_status.status = eRequestStatus::BUSY;
      // Increment common sequence ID and set it's value to current processing sequence ID for current request ID
      request_status.processing_server_seq_id = ++m_seq_id;
      // Store RequestInfo structure to request info set 
      const auto emplace_result = request_status.info_set.emplace( m_seq_id, seq_id, Address{ from_context, from_id } );
      if( false == emplace_result.second )
      {
         SYS_WRN( "emplace error: %s", m_seq_id.name( ).c_str( ) );
         return false;
      }
      return true;
   }

   template< typename TYPES >
   void MethodProcessor< TYPES >::process( const typename TYPES::method::tEvent& event )
   {
      m_processing_event_id = event.info( ).id( );
      if( true == pre_process( event ) )
         mp_server->process_request_event( event );
      m_processing_event_id.reset( );
   }

   template< typename TYPES >
   const comm::sequence::ID& MethodProcessor< TYPES >::unblock_request( )
   {
      if( std::nullopt == m_processing_event_id )
      {
         SYS_WRN( "unable unblock request" );
         return comm::sequence::ID::invalid;
      }

      // Find request id in request status map
      auto iterator_status_map = m_request_status_map.find( m_processing_event_id.value( ) );
      if( m_request_status_map.end( ) == iterator_status_map )
      {
         SYS_WRN( "not a request ID: %s", m_processing_event_id.value( ).c_str( ) );
         return comm::sequence::ID::invalid;
      }

      auto& request_status = iterator_status_map->second;
      request_status.status = eRequestStatus::READY;
      return request_status.processing_server_seq_id;
   }

   template< typename TYPES >
   void MethodProcessor< TYPES >::prepare_response( const comm::sequence::ID seq_id )
   {
      m_prepare_response_seq_id = seq_id;
   }

   template< typename TYPES >
   template< typename tResponseData, typename... Args >
   void MethodProcessor< TYPES >::response( const Args&... args )
   {
      // Find request id in request status map
      auto iterator_status_map = m_request_status_map.find( tResponseData::ID );
      if( m_request_status_map.end( ) == iterator_status_map )
      {
         SYS_WRN( "not a method ID: %s", tResponseData::ID.c_str( ) );
         return;
      }
      auto& request_status = iterator_status_map->second;
      auto& request_info_set = request_status.info_set;

      // Get searching sequence ID depending on 'm_prepare_response_seq_id' and 'processing_server_seq_id'
      comm::sequence::ID searching_seq_id = m_prepare_response_seq_id.value_or( request_status.processing_server_seq_id );
      // Search for RequestInfo structure in request info set for current searching sequence ID
      auto iterator_request_info_set = request_info_set.find( RequestInfo{ searching_seq_id } );
      if( request_info_set.end( ) == iterator_request_info_set )
      {
         SYS_WRN( "request info for currnet sequence id has not been found: %s", searching_seq_id.name( ).c_str( ) );
         return;
      }

      const RequestInfo& request_info = *iterator_request_info_set;

      typename TYPES::method::tEventUserSignature event_signature(
         mp_server->signature( ).role( ),
         tResponseData::ID,
         carpc::service::eType::RESPONSE,
         mp_server->id( ),
         request_info.client_addr.id( ),
         request_info.client_seq_id
      );
      typename TYPES::method::tEventData event_data( std::make_shared< tResponseData >( args... ) );
      TYPES::method::tEvent::create_send( event_signature, event_data, request_info.client_addr.context( ) );

      request_info_set.erase( iterator_request_info_set );
      request_status.status = eRequestStatus::READY;
      m_prepare_response_seq_id.reset( );
   }


} // namespace carpc::service::experimental::__private_server__



namespace carpc::service::experimental::__private_server__ {

   template< typename TYPES >
   struct NotificationStatus
   {
      using tServer = __private__::TServer< TYPES >;

      public:
         bool is_subscribed( ) const;
         bool add_subscriber( const application::Context&, const comm::service::ID& );
         bool remove_subscriber( const application::Context&, const comm::service::ID& );
         template< typename tAttributeData, typename... Args >
            void notify_subscribers( const tServer&, const Args& ... args );

      public:
         std::shared_ptr< typename TYPES::attribute::tBaseData > data( ) const;
      private:
         std::set< Address > subscribers;
         std::shared_ptr< typename TYPES::attribute::tBaseData > mp_data = nullptr;
   };

   template< typename TYPES >
   std::shared_ptr< typename TYPES::attribute::tBaseData > NotificationStatus< TYPES >::data( ) const
   {
      return mp_data;
   }

   template< typename TYPES >
   bool NotificationStatus< TYPES >::is_subscribed( ) const
   {
      return !( subscribers.empty( ) );
   }

   template< typename TYPES >
   bool NotificationStatus< TYPES >::add_subscriber( const application::Context& context, const comm::service::ID& service_id )
   {
      // In case of current process PID can have two different exact values but what can mean the same logic values.
      // It is current process ID and appliocation::process::local
      // Here we should standartize what exact value should be stored in collection.
      return subscribers.emplace(
                  Address{
                     application::Context{
                        context.tid( ),
                        context.is_external( ) ? context.pid( ) : application::process::local
                     },
                     service_id
                  }
               ).second;
   }

   template< typename TYPES >
   bool NotificationStatus< TYPES >::remove_subscriber( const application::Context& context, const comm::service::ID& service_id )
   {
      // In case of current process PID can have two different exact values but what can mean the same logic values.
      // It is current process ID and appliocation::process::local
      // Here we should standartize what exact value should be stored in collection.
      return 0 != subscribers.erase(
                     Address{
                        application::Context{
                           context.tid( ),
                           context.is_external( ) ? context.pid( ) : application::process::local
                        },
                        service_id
                     }
                  );
   }

   template< typename TYPES >
   template< typename tAttributeData, typename... Args >
   void NotificationStatus< TYPES >::notify_subscribers( const tServer& server, const Args& ... args )
   {
      mp_data = std::make_shared< tAttributeData >( args... );

      if( false == is_subscribed( ) )
      {
         SYS_VRB( "there are no subscribers to notify: %s", tAttributeData::ID.c_str( ) );
         return;
      }


      for( const auto& subscriber : subscribers )
      {
         typename TYPES::attribute::tEventUserSignature event_signature(
            server.signature( ).role( ),
            tAttributeData::ID,
            carpc::service::eType::NOTIFICATION,
            server.id( ),
            subscriber.id( )
         );
         typename TYPES::attribute::tEventData event_data( mp_data );
         TYPES::attribute::tEvent::create_send( event_signature, event_data, subscriber.context( ) );
      }
   }



   template< typename TYPES >
   class AttributeProcessor
   {
      using tServer = __private__::TServer< TYPES >;
      using tProxy = __private__::TProxy< TYPES >;

      using tNotificationStatus = NotificationStatus< TYPES >;
      using tAttributeStatusMap = std::map< typename TYPES::attribute::tEventID, tNotificationStatus >;

      public:
         AttributeProcessor( tServer* );
         ~AttributeProcessor( );

      public:
         bool process( const typename TYPES::attribute::tEvent& );
         template< typename tAttributeData, typename... Args >
            void notify( const Args&... args );
         template< typename tAttributeData >
            const tAttributeData* attribute( ) const;

      private:
         tServer*                mp_server = nullptr;
         tAttributeStatusMap     m_attribute_status_map;
   };

   template< typename TYPES >
   AttributeProcessor< TYPES >::AttributeProcessor( tServer* p_server )
      : mp_server( p_server )
   {
   }

   template< typename TYPES >
   AttributeProcessor< TYPES >::~AttributeProcessor( )
   {
   }

   template< typename TYPES >
   bool AttributeProcessor< TYPES >::process( const typename TYPES::attribute::tEvent& event )
   {
      const typename TYPES::attribute::tEventID event_id = event.info( ).id( );
      const carpc::service::eType event_type = event.info( ).type( );
      const comm::service::ID from_id = event.info( ).from( );
      const auto from_context = event.context( );

      if( carpc::service::eType::SUBSCRIBE == event_type )
      {
         // Processing attribute subscription event

         auto emplace_result = m_attribute_status_map.emplace( event_id, tNotificationStatus{ } );
         auto attribute_status_map_iterator = emplace_result.first;

         // In case there is cached attribute value => send notification event immidiatly to subscriber
         auto& notification_status = attribute_status_map_iterator->second;
         if( notification_status.add_subscriber( from_context, from_id ) )
         {
            // @TDA-DEBUG:
            SYS_VRB(
               "subscriber '%s' added for attribute '%s'",
               Address{ from_context, from_id }.name( ).c_str( ),
               event_id.c_str( )
            );
         }
         if( notification_status.data( ) )
         {
            typename TYPES::attribute::tEventUserSignature event_signature(
               mp_server->signature( ).role( ),
               event_id,
               carpc::service::eType::NOTIFICATION,
               mp_server->id( ),
               from_id
            );
            typename TYPES::attribute::tEventData event_data( notification_status.data( ) );
            TYPES::attribute::tEvent::create_send( event_signature, event_data, from_context );
         }
      }
      else if( carpc::service::eType::UNSUBSCRIBE == event_type )
      {
         // Processing attribute unsubscription event

         auto attribute_status_map_iterator = m_attribute_status_map.find( event_id );
         if( m_attribute_status_map.end( ) == attribute_status_map_iterator )
         {
            SYS_WRN( "there are no subscriptions for : %s", event_id.c_str( ) );
            return false;
         }

         auto& notification_status = attribute_status_map_iterator->second;
         if( notification_status.remove_subscriber( from_context, from_id ) )
         {
            // @TDA-DEBUG:
            SYS_VRB(
               "subscriber '%s' removed for attribute '%s'",
               Address{ from_context, from_id }.name( ).c_str( ),
               event_id.c_str( )
            );
         }
      }
      else
      {
         SYS_WRN( "unexpected event type: %s", event_type.c_str( ) );
         return false;
      }

      return true;
   }

   template< typename TYPES >
   template< typename tAttributeData, typename... Args >
   void AttributeProcessor< TYPES >::notify( const Args&... args )
   {
      auto emplace_result = m_attribute_status_map.emplace( tAttributeData::ID, tNotificationStatus{ } );
      if( false == emplace_result.second )
      {
         SYS_WRN( "emplace error: %s", tAttributeData::ID.c_str( ) );
         return;
      }

      auto attribute_status_map_iterator = emplace_result.first;
      auto& notification_status = attribute_status_map_iterator->second;
      notification_status.template notify_subscribers< tAttributeData >( *mp_server, args... );
   }

   template< typename TYPES >
   template< typename tAttributeData >
   const tAttributeData* AttributeProcessor< TYPES >::attribute( ) const
   {
      auto attribute_status_map_iterator = m_attribute_status_map.find( tAttributeData::ID );
      if( m_attribute_status_map.end( ) == attribute_status_map_iterator )
      {
         SYS_WRN( "not a notification ID: %s", tAttributeData::ID.c_str( ) );
         return nullptr;
      }

      const auto& notification_status = attribute_status_map_iterator->second;
      return static_cast< tAttributeData* >( notification_status.data( ).get( ) );
   }

} // namespace carpc::service::experimental::__private_server__



namespace carpc::service::experimental::__private__ {

   template< typename TYPES >
   class TServer
      : public IServer
      , public TYPES::method::tEventConsumer
      , public TYPES::attribute::tEventConsumer
   {
      using tServer = TServer< TYPES >;
      using tMethodProcessor = __private_server__::MethodProcessor< TYPES >;
      using tAttributeProcessor = __private_server__::AttributeProcessor< TYPES >;

      friend class __private_server__::MethodProcessor< TYPES >;

      public:
         TServer( const carpc::async::tAsyncTypeID&, const std::string&, const bool );
         ~TServer( ) override;

      private:
         void connected( const Address& ) override final;
         void disconnected( const Address& ) override final;
         void connected( ) override = 0;
         void disconnected( ) override = 0;

      protected:
         template< typename tResponseData, typename... Args >
            void response( const Args&... args );
         template< typename tAttributeData, typename... Args >
            void notify( const Args&... args );
         template< typename tAttributeData >
            const tAttributeData* attribute( ) const;
         template< typename tRequestData >
            const tRequestData* get_event_data( const typename TYPES::method::tEvent& event );
         const comm::sequence::ID& unblock_request( );
         void prepare_response( const comm::sequence::ID );

      private:
         void process_event( const typename TYPES::method::tEvent& ) override final;
         void process_event( const typename TYPES::attribute::tEvent& ) override final;
      private:
         virtual void process_request_event( const typename TYPES::method::tEvent& ) = 0;

      private:
         tMethodProcessor     m_method_processor;
         tAttributeProcessor  m_attribute_processor;
   };



   template< typename TYPES >
   TServer< TYPES >::TServer( const carpc::async::tAsyncTypeID& interface_type_id, const std::string& role_name, const bool is_export )
      : IServer( interface_type_id, role_name, is_export )
      , TYPES::method::tEventConsumer( )
      , TYPES::attribute::tEventConsumer( )
      , m_method_processor( this )
      , m_attribute_processor( this )
   {
      for( auto method_event_id : typename TYPES::method::tEventID( ) )
      {
         TYPES::method::tEvent::set_notification(
            this,
            typename TYPES::method::tEventUserSignature(
               signature( ).role( ),
               method_event_id,
               carpc::service::eType::REQUEST,
               comm::service::ID::invalid,
               id( )
            )
         );
      }
      for( auto attribute_event_id : typename TYPES::attribute::tEventID( ) )
      {
         TYPES::attribute::tEvent::set_notification(
            this,
            typename TYPES::attribute::tEventUserSignature(
               signature( ).role( ),
               attribute_event_id,
               carpc::service::eType::SUBSCRIBE,
               comm::service::ID::invalid,
               id( )
            )
         );

         TYPES::attribute::tEvent::set_notification(
            this,
            typename TYPES::attribute::tEventUserSignature(
               signature( ).role( ),
               attribute_event_id,
               carpc::service::eType::UNSUBSCRIBE,
               comm::service::ID::invalid,
               id( )
            )
         );
      }
   }

   template< typename TYPES >
   TServer< TYPES >::~TServer( )
   {
      TYPES::method::tEvent::clear_all_notifications( this );
      TYPES::attribute::tEvent::clear_all_notifications( this );
   }

   template< typename TYPES >
   void TServer< TYPES >::connected( const Address& proxy_address )
   {
      connected( );
   }

   template< typename TYPES >
   void TServer< TYPES >::disconnected( const Address& proxy_address )
   {
      disconnected( );
   }

   template< typename TYPES >
   void TServer< TYPES >::process_event( const typename TYPES::method::tEvent& event )
   {
      SYS_VRB( "processing event: %s", event.info( ).name( ).c_str( ) );
      m_method_processor.process( event );
   }

   template< typename TYPES >
   void TServer< TYPES >::process_event( const typename TYPES::attribute::tEvent& event )
   {
      SYS_VRB( "processing event: %s", event.info( ).name( ).c_str( ) );
      m_attribute_processor.process( event );
   }

   template< typename TYPES >
   const comm::sequence::ID& TServer< TYPES >::unblock_request( )
   {
      return m_method_processor.unblock_request( );
   }

   template< typename TYPES >
   void TServer< TYPES >::prepare_response( const comm::sequence::ID seq_id )
   {
      return m_method_processor.prepare_response( seq_id );
   }

   template< typename TYPES >
   template< typename tResponseData, typename... Args >
   void TServer< TYPES >::response( const Args&... args )
   {
      m_method_processor.template response< tResponseData >( args... );
   }

   template< typename TYPES >
   template< typename tAttributeData, typename... Args >
   void TServer< TYPES >::notify( const Args&... args )
   {
      m_attribute_processor.template notify< tAttributeData >( args... );
   }

   template< typename TYPES >
   template< typename tAttributeData >
   const tAttributeData* TServer< TYPES >::attribute( ) const
   {
      return m_attribute_processor.template attribute< tAttributeData >( );
   }

   template< typename TYPES >
   template< typename tRequestData >
   const tRequestData* TServer< TYPES >::get_event_data( const typename TYPES::method::tEvent& event )
   {
      if( const tRequestData* p_data = static_cast< tRequestData* >( event.data( )->ptr.get( ) ) )
         return p_data;

      SYS_ERR( "missing data for method ID: %s", event.info( ).id( ).c_str( ) );
      return nullptr;
   }

} // namespace carpc::service::experimental::__private__



namespace carpc::service::experimental {

   template< typename TYPES >
   class TServer : public __private__::TServer< TGenerator< TYPES > >
   {
      public:
         TServer( const std::string&, const bool );

         using tMethod = typename TGenerator< TYPES >::Method;
         using tAttribute = typename TGenerator< TYPES >::Attribute;
   };

   template< typename TYPES >
   TServer< TYPES >::TServer( const std::string& role_name, const bool is_export )
      : __private__::TServer< TGenerator< TYPES > >( TGenerator< TYPES >::interface_type_id, role_name, is_export )
   {
      REGISTER_EVENT( tMethod );
      REGISTER_EVENT( tAttribute );
   }

} // namespace carpc::service::experimental



#undef CLASS_ABBR
