#pragma once

#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/comm/interface/IInterface.hpp"
#include "api/sys/comm/interface/TClient.hpp"
#include "api/sys/service/ServiceProcess.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TProxy"



namespace base {

   template< typename TYPES >
      class TServer;
   template< typename TYPES >
      class TProxy;
   template< typename TYPES >
      class TClient;

}



namespace base {

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
      using tServer = TServer< TYPES >;
      using tProxy = TProxy< TYPES >;
      using tClient = TClient< TYPES >;
      using tRequestDB = RequestDB< TYPES >;

      public:
         RequestProcessor( tProxy* );
         void server( const tServer* const p_server );

      public:
         template< typename tRequestData, typename... Args >
            const tSequenceID request( tClient* p_client, const Args&... args );
         const bool response( const typename TYPES::tEvent& );

      private:
         tSequenceID                                        m_seq_id = 0;
         std::map< typename TYPES::tEventID, tRequestDB >   m_map;
         const tServer*                                     mp_server = nullptr;
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
   void RequestProcessor< TYPES >::server( const tServer* const p_server )
   {
      mp_server = p_server;
      m_map.clear( );
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
            TYPES::tEvent::set_notification( mp_proxy, typename TYPES::tSignature( mp_proxy->role( ), tRequestData::RESPONSE, mp_server, mp_proxy ) );
            TYPES::tEvent::set_notification( mp_proxy, typename TYPES::tSignature( mp_proxy->role( ), tRequestData::BUSY, mp_server, mp_proxy ) );
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

      typename TYPES::tEventData data( std::make_shared< tRequestData >( args... ) );
      TYPES::tEvent::create_send( typename TYPES::tSignature( mp_proxy->role( ), tRequestData::REQUEST, mp_proxy, mp_server, m_seq_id ), data, TYPES::COMM_TYPE );

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
               TYPES::tEvent::clear_notification( mp_proxy, typename TYPES::tSignature( mp_proxy->role( ), item.response, mp_server, mp_proxy ) );
            TYPES::tEvent::clear_notification( mp_proxy, typename TYPES::tSignature( mp_proxy->role( ), item.busy, mp_server, mp_proxy ) );
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



namespace base {

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
      using tServer = TServer< TYPES >;
      using tProxy = TProxy< TYPES >;
      using tClient = TClient< TYPES >;
      using tClientsSet = std::set< tClient* >;
      using tNotificationDB = NotificationDB< TYPES >;

      public:
         NotificationProcessor( tProxy* );
         void server( const tServer* const p_server );

      public:
         template< typename tNotificationData >
            const bool subscribe( tClient* );
         template< typename tNotificationData >
            const bool unsubscribe( tClient* );
         const bool notification( const typename TYPES::tEvent& );

      private:
         std::map< typename TYPES::tEventID, tNotificationDB > m_map;
         const tServer*                                        mp_server = nullptr;
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
   void NotificationProcessor< TYPES >::server( const tServer* const p_server )
   {
      mp_server = p_server;
      m_map.clear( );
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
         TYPES::tEvent::set_notification( mp_proxy, typename TYPES::tSignature( mp_proxy->role( ), tNotificationData::NOTIFICATION, mp_server, mp_proxy ) );
         TYPES::tEvent::create_send( typename TYPES::tSignature( mp_proxy->role( ), tNotificationData::SUBSCRIBE, mp_proxy, mp_server ), TYPES::COMM_TYPE );
      }
      clients_set.emplace( p_client );

      if( std::nullopt != event_id_iterator->second.m_event_data )
      {
         SYS_TRC( "having cached attribute event" );

         auto p_event = TYPES::tEvent::create( typename TYPES::tSignature( mp_proxy->role( ), tNotificationData::NOTIFICATION ) );
         p_event->data( event_id_iterator->second.m_event_data.value( ) );
         auto operation = [ p_client, p_event ]( ){ p_client->process_notification_event( *p_event ); };
         Runnable::create_send( operation );
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
         TYPES::tEvent::clear_notification( mp_proxy, typename TYPES::tSignature( mp_proxy->role( ), tNotificationData::NOTIFICATION, mp_server, mp_proxy ) );
         TYPES::tEvent::create_send( typename TYPES::tSignature( mp_proxy->role( ), tNotificationData::UNSUBSCRIBE, mp_proxy, mp_server ), TYPES::COMM_TYPE );
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



namespace base {

   template< typename TYPES >
   class TProxy
      : public IInterface
      , public TYPES::tEventConsumer
   {
      using tServer = TServer< TYPES >;
      using tClient = TClient< TYPES >;
      using tProxy = TProxy< TYPES >;
      using tRequestProcessor = RequestProcessor< TYPES >;
      using tNotificationProcessor = NotificationProcessor< TYPES >;
      using tAttributeMap = std::map< typename TYPES::tEventID, typename TYPES::tEvent >;

      private:
         TProxy( const std::string&, const std::string& );
         static std::map< TID, tProxy* > s_proxy_map;
      public:
         ~TProxy( ) override;
         static tProxy* create( const std::string&, const std::string& );

      public:
         void register_client( tClient* );
         void unregister_client( tClient* );

      private:
         void connected( const void* const ) override final;
         void disconnected( const void* const ) override final;

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

      public:
         const bool is_connected( ) const;
      private:
         const tServer*             mp_server = nullptr;

      private:
         std::set< tClient* >       m_client_set;
         tRequestProcessor          m_request_processor;
         tNotificationProcessor     m_notification_processor;
         tAttributeMap              m_attribute_map;
   };



   template< typename TYPES >
   TProxy< TYPES >::TProxy( const std::string& name, const std::string& role_name )
      : IInterface( name, role_name, eType::Client, TYPES::COMM_TYPE )
      , TYPES::tEventConsumer( )
      , m_request_processor( this )
      , m_notification_processor( this )
   {
      InterfaceEvent::Event::set_notification( this, { service_name( ), eInterface::ServerConnected } );
      InterfaceEvent::Event::create_send( { service_name( ), eInterface::ClientConnected }, { this }, TYPES::COMM_TYPE );
   }

   template< typename TYPES >
   TProxy< TYPES >::~TProxy( )
   {
      TYPES::tEvent::clear_all_notifications( this );
      InterfaceEvent::Event::clear_all_notifications( this );
      InterfaceEvent::Event::create_send( { service_name( ), eInterface::ClientDisconnected }, { this }, TYPES::COMM_TYPE );
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
   const bool TProxy< TYPES >::is_connected( ) const
   {
      return nullptr != mp_server;
   }

   template< typename TYPES >
   void TProxy< TYPES >::register_client( tClient* p_client )
   {
      if( nullptr == p_client )
         return;

      m_client_set.emplace( p_client );
      if( is_connected( ) )
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
      if( nullptr != mp_server )
         return;

      mp_server = reinterpret_cast< const tServer* >( p_server );
      m_request_processor.server( mp_server );
      m_notification_processor.server( mp_server );

      for( auto item : m_client_set )
         item->connected( );

      InterfaceEvent::Event::clear_notification( this, { service_name( ), eInterface::ServerConnected } );
      InterfaceEvent::Event::set_notification( this, { service_name( ), eInterface::ServerDisconnected } );
      InterfaceEvent::Event::create_send( { service_name( ), eInterface::ClientConnected }, { this }, TYPES::COMM_TYPE );
   }

   template< typename TYPES >
   void TProxy< TYPES >::disconnected( const void* const p_server )
   {
      m_request_processor.server( nullptr );
      m_notification_processor.server( nullptr );

      mp_server = nullptr;
      for( auto item : m_client_set )
         item->disconnected( );

      InterfaceEvent::Event::set_notification( this, { service_name( ), eInterface::ServerConnected } );
      InterfaceEvent::Event::clear_notification( this, { service_name( ), eInterface::ServerDisconnected } );
   }

   template< typename TYPES >
   void TProxy< TYPES >::process_event( const typename TYPES::tEvent& event )
   {
      const typename TYPES::tEventID event_id = event.info( ).id( );
      const void* p_from_addr = event.info( ).from_addr( );
      const void* p_to_addr = event.info( ).to_addr( );
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

} // namespace base



#undef CLASS_ABBR
