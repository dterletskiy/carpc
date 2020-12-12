#pragma once

#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/application/Process.hpp"
#include "api/sys/comm/service/IProxy.hpp"
#include "api/sys/comm/service/fast/TClient.hpp"
#include "api/sys/comm/service/fast/TSignature.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TProxyFast"



namespace base::service::fast {

   template< typename TYPES >
      class TServer;
   template< typename TYPES >
      class TProxy;
   template< typename TYPES >
      class TClient;

} // namespace base::service::fast



namespace base::service::fast {

   template< typename TYPES >
   struct RequestDB
   {
      using tClient = TClient< TYPES >;

      size_t                              m_count = 0;
      std::map< tSequenceID, tClient* >   m_client_map;
   };



   template< typename TYPES >
   class RequestProcessor
   {
      using tProxy = TProxy< TYPES >;
      using tClient = TClient< TYPES >;
      using tRequestDB = RequestDB< TYPES >;

      public:
         RequestProcessor( tProxy* );
         void reset( );

      public:
         template< typename tRequestData, typename... Args >
            const tSequenceID request( tClient* p_client, const Args&... args );
         const bool response( const typename TYPES::tEvent& );

      private:
         tSequenceID                                        m_seq_id = tSequenceID::zero( );
         std::map< typename TYPES::tEventID, tRequestDB >   m_map;
         tProxy*                                            mp_proxy = nullptr;
   };



   template< typename TYPES >
   RequestProcessor< TYPES >::RequestProcessor( tProxy* p_proxy )
      : mp_proxy( p_proxy )
   {
      for( auto item : TYPES::RR )
      {
         m_map.emplace( std::make_pair( item.request, tRequestDB{ } ) );
      }
   }

   template< typename TYPES >
   void RequestProcessor< TYPES >::reset( )
   {
      for( auto item : m_map )
         item.second = tRequestDB{ };
   }

   template< typename TYPES >
   template< typename tRequestData, typename... Args >
   const tSequenceID RequestProcessor< TYPES >::request( tClient* p_client, const Args&... args )
   {
      auto event_id_iterator = m_map.find( tRequestData::REQUEST );
      if( m_map.end( ) == event_id_iterator )
      {
         SYS_WRN( "request id does not exist: %s", to_string( tRequestData::REQUEST ).c_str( ) );
         return InvalidSequenceID;
      }

      // If response exists for current request
      if( TYPES::tEventID::Undefined != tRequestData::RESPONSE )
      {
         size_t& count = event_id_iterator->second.m_count;
         // In case if count for current request id is zero this means current proxy is not subscribed for corresponding responses of mentioned request
         if( 0 == count )
         {
            TYPES::tEvent::set_notification(
               mp_proxy,
               typename TYPES::tSignature(
                  mp_proxy->signature( ).role( ),
                  tRequestData::RESPONSE,
                  mp_proxy->server( ).id( ),
                  mp_proxy->id( )
               )
            );
            TYPES::tEvent::set_notification(
               mp_proxy,
               typename TYPES::tSignature(
                  mp_proxy->signature( ).role( ),
                  tRequestData::BUSY,
                  mp_proxy->server( ).id( ),
                  mp_proxy->id( )
               )
            );
         }
         ++count;

         // Incrementing common sequence id for all requests in this proxy. This sequence id will be sent to server in event.
         // Also new sequence id and corresponding client pointer are added to the map for later client identifying in response by sequence id
         // received from server.
         auto& client_map = event_id_iterator->second.m_client_map;
         auto result = client_map.emplace( ++m_seq_id, p_client );
         if( false == result.second )
         {
            SYS_WRN( "can not insert: %s -> %p", m_seq_id.name( ).c_str( ), p_client );
            return InvalidSequenceID;
         }
      }

      typename TYPES::tSignature event_signature(
            mp_proxy->signature( ).role( ),
            tRequestData::REQUEST,
            mp_proxy->id( ),
            mp_proxy->server( ).id( ),
            m_seq_id
         );
      typename TYPES::tEventData data( std::make_shared< tRequestData >( args... ) );
      TYPES::tEvent::create_send( event_signature, data, mp_proxy->server( ).context( ) );

      return m_seq_id;
   }

   template< typename TYPES >
   const bool RequestProcessor< TYPES >::response( const typename TYPES::tEvent& event )
   {
      const typename TYPES::tEventID event_id = event.info( ).id( );
      const tSequenceID seq_id = event.info( ).seq_id( );

      for( auto& item : TYPES::RR )
      {
         if( item.response != event_id && item.busy != event_id )
            continue;

         auto event_id_iterator = m_map.find( item.request );
         if( m_map.end( ) == event_id_iterator )
         {
            SYS_WRN( "request id does not exist: %s", to_string( item.request ).c_str( ) );
            return false;
         }

         size_t& count = event_id_iterator->second.m_count;
         --count;
         // In case if count for current request is zero this means current proxy is not interested in following response notifications
         if( 0 == count )
         {
            if( TYPES::tEventID::Undefined != item.response )
            {
               TYPES::tEvent::clear_notification(
                     mp_proxy,
                     typename TYPES::tSignature(
                        mp_proxy->signature( ).role( ),
                        item.response,
                        mp_proxy->server( ).id( ),
                        mp_proxy->id( )
                     )
                  );
            }
            TYPES::tEvent::clear_notification(
                  mp_proxy,
                  typename TYPES::tSignature(
                     mp_proxy->signature( ).role( ),
                     item.busy,
                     mp_proxy->server( ).id( ),
                     mp_proxy->id( )
                  )
               );
         }

         auto& client_map = event_id_iterator->second.m_client_map;
         auto seq_id_iterator = client_map.find( seq_id );
         if( client_map.end( ) == seq_id_iterator )
         {
            SYS_WRN( "delivered event to unknown client" );
            return false;
         }
         seq_id_iterator->second->process_response_event( event );
         client_map.erase( seq_id_iterator );

         return true;
      }

      // SYS_WRN( "related request id has not been found for %s", to_string( event_id ).c_str( ) );
      return false;
   }

} // namespace base::service::fast



namespace base::service::fast {

   template< typename TYPES >
   struct NotificationDB
   {
      using tClient = TClient< TYPES >;
      using tClientsSet = std::set< tClient* >;

      tClientsSet                                        m_client_set;
      std::shared_ptr< typename TYPES::tEventData >      mp_event_data = nullptr;
   };



   template< typename TYPES >
   class NotificationProcessor
   {
      using tProxy = TProxy< TYPES >;
      using tClient = TClient< TYPES >;
      using tClientsSet = std::set< tClient* >;
      using tNotificationDB = NotificationDB< TYPES >;

      public:
         NotificationProcessor( tProxy* );
         void reset( );

      public:
         template< typename tNotificationData >
            const bool subscribe( tClient* );
         template< typename tNotificationData >
            const bool unsubscribe( tClient* );
         const bool notification( const typename TYPES::tEvent& );

      private:
         std::map< typename TYPES::tEventID, tNotificationDB > m_map;
         tProxy*                                               mp_proxy = nullptr;
   };



   template< typename TYPES >
   NotificationProcessor< TYPES >::NotificationProcessor( tProxy* p_proxy )
      : mp_proxy( p_proxy )
   {
      for( auto item : TYPES::N )
      {
         m_map.emplace( std::make_pair( item.notification, tNotificationDB{ } ) );
      }
   }

   template< typename TYPES >
   void NotificationProcessor< TYPES >::reset( )
   {
      for( auto item : m_map )
         item.second = tNotificationDB{ };
   }

   template< typename TYPES >
   template< typename tNotificationData >
   const bool NotificationProcessor< TYPES >::subscribe( tClient* p_client )
   {
      auto event_id_iterator = m_map.find( tNotificationData::NOTIFICATION );
      if( m_map.end( ) == event_id_iterator )
      {
         SYS_WRN( "notification id does not exist: %s", to_string( tNotificationData::NOTIFICATION ).c_str( ) );
         return false;
      }

      tClientsSet& clients_set = event_id_iterator->second.m_client_set;
      if( clients_set.empty( ) )
      {
         TYPES::tEvent::set_notification(
               mp_proxy,
               typename TYPES::tSignature(
                  mp_proxy->signature( ).role( ),
                  tNotificationData::NOTIFICATION,
                  mp_proxy->server( ).id( ),
                  mp_proxy->id( )
               )
            );
         TYPES::tEvent::create_send(
               typename TYPES::tSignature(
                  mp_proxy->signature( ).role( ),
                  tNotificationData::SUBSCRIBE,
                  mp_proxy->id( ),
                  mp_proxy->server( ).id( )
               ),
               mp_proxy->server( ).context( )
            );
      }
      clients_set.emplace( p_client );

      if( nullptr != event_id_iterator->second.mp_event_data )
      {
         SYS_TRC( "having cached attribute event" );

         auto p_event = TYPES::tEvent::create(
               typename TYPES::tSignature(
                     mp_proxy->signature( ).role( ),
                     tNotificationData::NOTIFICATION
                  )
            );
         p_event->data( event_id_iterator->second.mp_event_data );
         auto operation = [ p_client, p_event ]( ){ p_client->process_notification_event( *p_event ); };
         base::async::Runnable::create_send( operation );
      }

      return true;
   }

   template< typename TYPES >
   template< typename tNotificationData >
   const bool NotificationProcessor< TYPES >::unsubscribe( tClient* p_client )
   {
      auto event_id_iterator = m_map.find( tNotificationData::NOTIFICATION );
      if( m_map.end( ) == event_id_iterator )
      {
         SYS_WRN( "notification id does not exist: %s", to_string( tNotificationData::NOTIFICATION ).c_str( ) );
         return false;
      }

      tClientsSet& clients_set = event_id_iterator->second.m_client_set;
      clients_set.erase( p_client );
      if( clients_set.empty( ) )
      {
         TYPES::tEvent::clear_notification(
            mp_proxy,
            typename TYPES::tSignature(
                  mp_proxy->signature( ).role( ),
                  tNotificationData::NOTIFICATION,
                  mp_proxy->server( ).id( ),
                  mp_proxy->id( )
               )
         );
         TYPES::tEvent::create_send(
            typename TYPES::tSignature(
               mp_proxy->signature( ).role( ),
               tNotificationData::UNSUBSCRIBE,
               mp_proxy->id( ),
               mp_proxy->server( ).id( )
            ),
            mp_proxy->server( ).context( )
         );
         event_id_iterator->second.mp_event_data = nullptr;
      }

      return true;
   }

   template< typename TYPES >
   const bool NotificationProcessor< TYPES >::notification( const typename TYPES::tEvent& event )
   {
      const typename TYPES::tEventID event_id = event.info( ).id( );

      auto event_id_iterator = m_map.find( event_id );
      if( m_map.end( ) == event_id_iterator )
      {
         // SYS_WRN( "notification id does not exist: %s", to_string( event_id ).c_str( ) );
         return false;
      }

      tNotificationDB& notification_db = event_id_iterator->second;

      notification_db.mp_event_data = event.data( );

      for( auto p_client : notification_db.m_client_set )
         p_client->process_notification_event( event );

      return true;
   }

} // namespace base::service::fast



namespace base::service::fast {

   template< typename TYPES >
   class TProxy
      : public IProxy
      , public TYPES::tEventConsumer
   {
      using tClient = TClient< TYPES >;
      using tProxy = TProxy< TYPES >;
      using tRequestProcessor = RequestProcessor< TYPES >;
      using tNotificationProcessor = NotificationProcessor< TYPES >;
      using tAttributeMap = std::map< typename TYPES::tEventID, typename TYPES::tEvent >;

      private:
         TProxy( const base::async::tAsyncTypeID&, const std::string&, const bool );
         static thread_local tProxy* sp_proxy;
      public:
         ~TProxy( ) override;
         static tProxy* create( const base::async::tAsyncTypeID&, const std::string&, const bool );

      private:
         void connected( const Address& ) override final;
         void disconnected( const Address& ) override final;
         void connected( ) override final { }
         void disconnected( ) override final { }

      public:
         void test( ) { }

      private:
         void process_event( const typename TYPES::tEvent& ) override final;

      public:
         template< typename tRequestData, typename... Args >
            const tSequenceID request( tClient*, const Args&... args );
         template< typename tNotificationData >
            const bool subscribe( tClient* );
         template< typename tNotificationData >
            const bool unsubscribe( tClient* );
         template< typename tResponseData >
            const tResponseData* get_event_data( const typename TYPES::tEvent& event );

      private:
         tRequestProcessor          m_request_processor;
         tNotificationProcessor     m_notification_processor;
   };



   template< typename TYPES >
   TProxy< TYPES >::TProxy( const base::async::tAsyncTypeID& interface_type_id, const std::string& role_name, const bool is_import )
      : IProxy( interface_type_id, role_name, is_import )
      , TYPES::tEventConsumer( )
      , m_request_processor( this )
      , m_notification_processor( this )
   {
   }

   template< typename TYPES >
   TProxy< TYPES >::~TProxy( )
   {
      TYPES::tEvent::clear_all_notifications( this );
   }

   template< typename TYPES >
   thread_local TProxy< TYPES >* TProxy< TYPES >::sp_proxy = nullptr;

   template< typename TYPES >
   TProxy< TYPES >* TProxy< TYPES >::create( const base::async::tAsyncTypeID& interface_type_id, const std::string& role_name, const bool is_import )
   {
      if( nullptr != sp_proxy )
      {
         SYS_TRC( "return existing proxy: %p", sp_proxy );
         return sp_proxy;
      }

      if( application::Process::instance( )->current_thread( )->id( ).is_invalid( ) )
      {
         SYS_ERR( "creating proxy '%s.%s' not from application thread", interface_type_id.name( ).c_str( ), role_name.c_str( ) );
         return nullptr;
      }

      sp_proxy = new tProxy( interface_type_id, role_name, is_import );
      if( nullptr == sp_proxy )
      {
         SYS_ERR( "unable create proxy" );
         return nullptr;
      }
      SYS_TRC( "proxy created: %p", sp_proxy );

      return sp_proxy;
   }

   template< typename TYPES >
   void TProxy< TYPES >::connected( const Address& server_address )
   {
      m_request_processor.reset( );
      m_notification_processor.reset( );
   }

   template< typename TYPES >
   void TProxy< TYPES >::disconnected( const Address& server_address )
   {
      m_request_processor.reset( );
      m_notification_processor.reset( );
   }

   template< typename TYPES >
   void TProxy< TYPES >::process_event( const typename TYPES::tEvent& event )
   {
      const typename TYPES::tEventID event_id = event.info( ).id( );
      const service::ID from_id = event.info( ).from( );
      const service::ID to_id = event.info( ).to( );
      const tSequenceID seq_id = event.info( ).seq_id( );
      const auto from_context = event.context( );

      SYS_TRC( "processing event: %s", event.info( ).name( ).c_str( ) );

      if( true == m_request_processor.response( event ) )
         return;
      if( true == m_notification_processor.notification( event ) )
         return;

      SYS_WRN( "unknown event" );
   }

   template< typename TYPES >
   template< typename tRequestData, typename... Args >
   const tSequenceID TProxy< TYPES >::request( tClient* p_client, const Args&... args )
   {
      if( !is_connected( ) )
      {
         SYS_WRN( "proxy is not connected" );
         return InvalidSequenceID;
      }

      return m_request_processor.template request< tRequestData >( p_client, args... );
   }

   template< typename TYPES >
   template< typename tNotificationData >
   const bool TProxy< TYPES >::subscribe( tClient* p_client )
   {
      if( !is_connected( ) )
      {
         SYS_WRN( "proxy is not connected" );
         return false;
      }

      return m_notification_processor.template subscribe< tNotificationData >( p_client );
   }

   template< typename TYPES >
   template< typename tNotificationData >
   const bool TProxy< TYPES >::unsubscribe( tClient* p_client )
   {
      return m_notification_processor.template unsubscribe< tNotificationData >( p_client );
   }

   template< typename TYPES >
   template< typename tResponseData >
   const tResponseData* TProxy< TYPES >::get_event_data( const typename TYPES::tEvent& event )
   {
      if( const tResponseData* p_data = static_cast< tResponseData* >( event.data( )->ptr.get( ) ) )
         return p_data;

      SYS_ERR( "missing data for response/notification ID: %s", to_string( event.info( ).id( ) ).c_str( ) );
      return nullptr;
   }

} // namespace base::service::fast



#undef CLASS_ABBR
