#pragma once

#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/comm/interface/IProxy.hpp"
#include "api/sys/comm/interface/TClient.hpp"
#include "api/sys/service/ServiceProcess.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TProxy"



namespace base::interface {

   template< typename TYPES >
      class TServer;
   template< typename TYPES >
      class TProxy;
   template< typename TYPES >
      class TClient;

}



namespace base::interface {

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
         void server( const Address::tOpt& );

      public:
         template< typename tRequestData, typename... Args >
            const tSequenceID request( tClient* p_client, const Args&... args );
         const bool response( const typename TYPES::tEvent& );

      private:
         tSequenceID                                        m_seq_id = 0;
         std::map< typename TYPES::tEventID, tRequestDB >   m_map;
         Address::tOpt                                      m_server = std::nullopt;
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
   void RequestProcessor< TYPES >::server( const Address::tOpt& server )
   {
      m_server = server;
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
                  mp_proxy->server( ).value( ),
                  Address{ mp_proxy }
               )
            );
            TYPES::tEvent::set_notification(
               mp_proxy,
               typename TYPES::tSignature(
                  mp_proxy->signature( ).role( ),
                  tRequestData::BUSY,
                  mp_proxy->server( ).value( ),
                  Address{ mp_proxy }
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
            SYS_WRN( "can not insert: %zu -> %p", m_seq_id, p_client );
            return InvalidSequenceID;
         }
      }

      typename TYPES::tSignature event_signature(
            mp_proxy->signature( ).role( ),
            tRequestData::REQUEST,
            Address{ mp_proxy },
            mp_proxy->server( ).value( ),
            m_seq_id
         );
      typename TYPES::tEventData data( std::make_shared< tRequestData >( args... ) );
      TYPES::tEvent::create_send( event_signature, data, TYPES::COMM_TYPE );

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
                        mp_proxy->server( ).value( ),
                        Address{ mp_proxy }
                     )
                  );
            }
            TYPES::tEvent::clear_notification(
                  mp_proxy,
                  typename TYPES::tSignature(
                     mp_proxy->signature( ).role( ),
                     item.busy,
                     mp_proxy->server( ).value( ),
                     Address{ mp_proxy }
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

}



namespace base::interface {

   template< typename TYPES >
   struct NotificationDB
   {
      using tClient = TClient< TYPES >;
      using tClientsSet = std::set< tClient* >;

      tClientsSet                                                    m_client_set;
      std::optional< std::shared_ptr< typename TYPES::tEventData > > m_event_data = std::nullopt;
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
         void server( const Address::tOpt& );

      public:
         template< typename tNotificationData >
            const bool subscribe( tClient* );
         template< typename tNotificationData >
            const bool unsubscribe( tClient* );
         const bool notification( const typename TYPES::tEvent& );

      private:
         std::map< typename TYPES::tEventID, tNotificationDB > m_map;
         Address::tOpt                                         m_server = std::nullopt;
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
   void NotificationProcessor< TYPES >::server( const Address::tOpt& server )
   {
      m_server = server;
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
                  mp_proxy->server( ).value( ),
                  Address{ mp_proxy }
               )
            );
         TYPES::tEvent::create_send(
               typename TYPES::tSignature(
                  mp_proxy->signature( ).role( ),
                  tNotificationData::SUBSCRIBE,
                  Address{ mp_proxy },
                  mp_proxy->server( ).value( )
               ),
               TYPES::COMM_TYPE
            );
      }
      clients_set.emplace( p_client );

      if( std::nullopt != event_id_iterator->second.m_event_data )
      {
         SYS_TRC( "having cached attribute event" );

         auto p_event = TYPES::tEvent::create(
               typename TYPES::tSignature(
                     mp_proxy->signature( ).role( ),
                     tNotificationData::NOTIFICATION,
                     Address{ nullptr },
                     Address{ nullptr }
                  )
            );
         p_event->data( event_id_iterator->second.m_event_data.value( ) );
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
                  mp_proxy->server( ).value( ),
                  Address{ mp_proxy }
               )
         );
         TYPES::tEvent::create_send(
            typename TYPES::tSignature(
               mp_proxy->signature( ).role( ),
               tNotificationData::UNSUBSCRIBE,
               Address{ mp_proxy },
               mp_proxy->server( ).value( )
            ),
            TYPES::COMM_TYPE
         );
         event_id_iterator->second.m_event_data = std::nullopt;
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

      for( auto p_client : event_id_iterator->second.m_client_set )
         p_client->process_notification_event( event );

      return true;
   }

}



namespace base::interface {

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
         static std::map< ID, tProxy* > s_proxy_map;
      public:
         ~TProxy( ) override;
         static tProxy* create( const base::async::tAsyncTypeID&, const std::string&, const bool );

      private:
         void connected( ) override final;
         void disconnected( ) override final;

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
         tAttributeMap              m_attribute_map;
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
   std::map< ID, TProxy< TYPES >* > TProxy< TYPES >::s_proxy_map;

   template< typename TYPES >
   TProxy< TYPES >* TProxy< TYPES >::create( const base::async::tAsyncTypeID& interface_type_id, const std::string& role_name, const bool is_import )
   {
      ID tid = ServiceProcess::instance( )->current_service( )->id( );
      os::Mutex mutex( true );

      auto iterator = s_proxy_map.find( tid );
      if( s_proxy_map.end( ) != iterator )
      {
         SYS_TRC( "return existing proxy: %p", iterator->second );
         return iterator->second;
      }

      tProxy* p_proxy = new tProxy( interface_type_id, role_name, is_import );
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
   void TProxy< TYPES >::connected( )
   {
      m_request_processor.server( m_server );
      m_notification_processor.server( m_server );
   }

   template< typename TYPES >
   void TProxy< TYPES >::disconnected( )
   {
      m_request_processor.server( std::nullopt );
      m_notification_processor.server( std::nullopt );
   }

   template< typename TYPES >
   void TProxy< TYPES >::process_event( const typename TYPES::tEvent& event )
   {
      const typename TYPES::tEventID event_id = event.info( ).id( );
      const Address& from_addr = event.info( ).from_addr( );
      const Address& to_addr = event.info( ).to_addr( );
      const tSequenceID seq_id = event.info( ).seq_id( );

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
         return InvalidSequenceID;
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
      return static_cast< tResponseData* >( event.data( )->ptr.get( ) );
   }

} // namespace base::interface



#undef CLASS_ABBR
