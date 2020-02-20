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
      class TProxy;



   template< typename TYPES >
   struct ClientsCount
   {
      using tClient = TClient< TYPES >;

      size_t                  m_count = 0;
      std::set< tClient* >    m_client_set;
   };



   template< typename TYPES >
   class RequestCount
   {
      using tProxy = TProxy< TYPES >;
      using tClient = TClient< TYPES >;

   public:
      RequestCount( tProxy* );

   public:
      template< typename tRequestData >
         bool request( );
      bool response( const typename TYPES::tEventID& );

   private:
      std::map< typename TYPES::tEventID, size_t >    m_map;
      tProxy*                                         mp_proxy = nullptr;
   };



   template< typename TYPES >
   RequestCount< TYPES >::RequestCount( tProxy* p_proxy )
      : mp_proxy( p_proxy )
   {
      for( auto item : TYPES::RR )
      {
         m_map.emplace( std::make_pair( item.request, 0 ) );
      }
   }

   template< typename TYPES >
   template< typename tRequestData >
   bool RequestCount< TYPES >::request( )
   {
      auto iterator = m_map.find( tRequestData::REQUEST );
      if( m_map.end( ) == iterator )
      {
         SYS_WRN( "request id does not exist: %s", to_string( tRequestData::REQUEST ).c_str( ) );
         return false;
      }

      size_t& count = iterator->second;
      if( 0 == count )
      {
         if( TYPES::tEventID::Undefined != tRequestData::RESPONSE )
            TYPES::tEvent::set_notification( mp_proxy, typename TYPES::tEvent::Signature( mp_proxy->role( ), tRequestData::RESPONSE, nullptr, nullptr, 0 ) );
         TYPES::tEvent::set_notification( mp_proxy, typename TYPES::tEvent::Signature( mp_proxy->role( ), tRequestData::BUSY, nullptr, nullptr, 0 ) );
      }

      ++count;
      return true;
   }

   template< typename TYPES >
   bool RequestCount< TYPES >::response( const typename TYPES::tEventID& event_id )
   {
      for( auto& item : TYPES::RR )
      {
         if( item.response != event_id && item.busy != event_id )
            continue;

         auto iterator = m_map.find( item.request );
         if( m_map.end( ) == iterator )
         {
            SYS_WRN( "request id does not exist: %s", to_string( item.request ).c_str( ) );
            return false;
         }

         size_t& count = iterator->second;
         --count;
         if( 0 == count )
         {
            if( TYPES::tEventID::Undefined != item.response )
               TYPES::tEvent::clear_notification( mp_proxy, typename TYPES::tEvent::Signature( mp_proxy->role( ), item.response, nullptr, nullptr, 0 ) );
            TYPES::tEvent::clear_notification( mp_proxy, typename TYPES::tEvent::Signature( mp_proxy->role( ), item.busy, nullptr, nullptr, 0 ) );
         }

         return true;
      }

      SYS_WRN( "related request id has not been found for %s", to_string( event_id ).c_str( ) );
      return false;
   }



   template< typename TYPES >
   struct NotificationDB
   {
      using tClient = TClient< TYPES >;
      using tClientsSet = std::set< tClient* >;

      tClientsSet                                                    m_client_set;
      std::optional< std::shared_ptr< typename TYPES::tEventData > > m_event_data = std::nullopt;
   };



   template< typename TYPES >
   class NotificationCount
   {
      using tProxy = TProxy< TYPES >;
      using tClient = TClient< TYPES >;
      using tClientsSet = std::set< tClient* >;
      using tNotificationDB = NotificationDB< TYPES >;

   public:
      NotificationCount( tProxy* );

   public:
      template< typename tNotificationData >
         const bool subscribe( tClient* );
      template< typename tNotificationData >
         const bool unsubscribe( tClient* );
      const std::set< tClient* >* notification( const typename TYPES::tEvent& );

   private:
      std::map< typename TYPES::tEventID, tNotificationDB > m_map;
      tProxy*                                               mp_proxy = nullptr;
   };



   template< typename TYPES >
   NotificationCount< TYPES >::NotificationCount( tProxy* p_proxy )
      : mp_proxy( p_proxy )
   {
      for( auto item : TYPES::N )
      {
         m_map.emplace( std::make_pair( item.notification, tNotificationDB{ } ) );
      }
   }

   template< typename TYPES >
   template< typename tNotificationData >
   const bool NotificationCount< TYPES >::subscribe( tClient* p_client )
   {
      auto iterator = m_map.find( tNotificationData::NOTIFICATION );
      if( m_map.end( ) == iterator )
      {
         SYS_WRN( "notification id does not exist: %s", to_string( tNotificationData::NOTIFICATION ).c_str( ) );
         return false;
      }

      tClientsSet& clients_set = iterator->second.m_client_set;
      if( clients_set.empty( ) )
      {
         TYPES::tEvent::set_notification( mp_proxy, typename TYPES::tEvent::Signature( mp_proxy->role( ), tNotificationData::NOTIFICATION, nullptr, nullptr, 0 ) );
         TYPES::tEvent::create_send( typename TYPES::tEvent::Signature( mp_proxy->role( ), tNotificationData::SUBSCRIBE, nullptr, nullptr, 0 ), TYPES::COMM_TYPE );
      }
      clients_set.emplace( p_client );

      if( std::nullopt != iterator->second.m_event_data )
      {
         SYS_TRC( "having cached attribute event" );

         auto p_event = TYPES::tEvent::create( typename TYPES::tEvent::Signature( mp_proxy->role( ), tNotificationData::NOTIFICATION, nullptr, p_client, 0 ) );
         p_event->data( iterator->second.m_event_data.value( ) );
         p_event->send( TYPES::COMM_TYPE );
      }

      return true;
   }

   template< typename TYPES >
   template< typename tNotificationData >
   const bool NotificationCount< TYPES >::unsubscribe( tClient* p_client )
   {
      auto iterator = m_map.find( tNotificationData::NOTIFICATION );
      if( m_map.end( ) == iterator )
      {
         SYS_WRN( "notification id does not exist: %s", to_string( tNotificationData::NOTIFICATION ).c_str( ) );
         return false;
      }

      tClientsSet& clients_set = iterator->second.m_client_set;
      clients_set.erase( p_client );
      if( clients_set.empty( ) )
      {
         TYPES::tEvent::clear_notification( mp_proxy, typename TYPES::tEvent::Signature( mp_proxy->role( ), tNotificationData::NOTIFICATION, nullptr, nullptr, 0 ) );
         TYPES::tEvent::create_send( typename TYPES::tEvent::Signature( mp_proxy->role( ), tNotificationData::UNSUBSCRIBE, nullptr, nullptr, 0 ), TYPES::COMM_TYPE );
         iterator->second.m_event_data = std::nullopt;
      }

      return true;
   }

   template< typename TYPES >
   const std::set< TClient< TYPES >* >* NotificationCount< TYPES >::notification( const typename TYPES::tEvent& event )
   {
      const typename TYPES::tEventID event_id = event.info( ).id( );
      const void* p_to_addr = event.info( ).to_addr( );

      auto iterator = m_map.find( event_id );
      if( m_map.end( ) == iterator )
      {
         SYS_WRN( "notification id does not exist: %s", to_string( event_id ).c_str( ) );
         return nullptr;
      }

      iterator->second.m_event_data = event.data( );
      if( nullptr != p_to_addr )
      {
         static tClientsSet s_client_set;
         s_client_set = { (tClient*)p_to_addr };
         return &s_client_set;
      }

      return &( iterator->second.m_client_set );
   }

}



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
   using tRequestCount = RequestCount< TYPES >;
   using tNotificationCount = NotificationCount< TYPES >;

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
      const SequenceID request( tClient*, const Args&... args );
   template< typename tNotificationData >
      const bool subscribe( tClient* );
   template< typename tNotificationData >
      const bool unsubscribe( tClient* );
   template< typename tResponseData >
      const tResponseData* get_event_data( const typename TYPES::tEvent& event );

public:
   const bool is_connected( ) const;
private:
   const tServer*                                              mp_server = nullptr;

private:
   std::set< tClient* >                                           m_client_set;
   SequenceID                                                     m_seq_id = 0;
   tRequestCount                                                  m_request_count;
   tNotificationCount                                             m_notification_count;
   std::map< typename TYPES::tEventID, typename TYPES::tEvent >   m_attribute_map;
};



template< typename TYPES >
TProxy< TYPES >::TProxy( const std::string& name, const std::string& role_name )
   : IInterface( name, role_name )
   , TYPES::tEventConsumer( )
   , m_request_count( this )
   , m_notification_count( this )
{
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

   if( true == m_request_count.response( event_id ) )
   {
      auto iterator = m_client_set.find( (tClient*)p_to_addr );
      if( m_client_set.end( ) == iterator )
      {
         SYS_WRN( "delivered event to unknown client: %p", p_to_addr )
         return;
      }
      (*iterator)->process_response_event( event );
      return;
   }

   ;
   if( const std::set< tClient* >* p_clients_set = m_notification_count.notification( event ) )
   {
      for( auto p_client : *p_clients_set )
      {
         p_client->process_notification_event( event );
      }
      return;
   }

   SYS_WRN( "unknown event" );
}

template< typename TYPES >
template< typename tRequestData, typename... Args >
const SequenceID TProxy< TYPES >::request( tClient* p_client, const Args&... args )
{
   if( !is_connected( ) )
      return InvalidSequenceID;

   if( false == m_request_count.template request< tRequestData >( ) )
      return InvalidSequenceID;

   typename TYPES::tEventData data( std::make_shared< tRequestData >( args... ) );
   TYPES::tEvent::create_send( typename TYPES::tEvent::Signature( role( ), tRequestData::REQUEST, p_client, mp_server, ++m_seq_id ), data, TYPES::COMM_TYPE );

   return m_seq_id;
}

template< typename TYPES >
template< typename tNotificationData >
const bool TProxy< TYPES >::subscribe( tClient* p_client )
{
   // if( !is_connected( ) )
   //    return false;

   return m_notification_count.template subscribe< tNotificationData >( p_client );
}

template< typename TYPES >
template< typename tNotificationData >
const bool TProxy< TYPES >::unsubscribe( tClient* p_client )
{
   // if( !is_connected( ) )
   //    return false;

   return m_notification_count.template unsubscribe< tNotificationData >( p_client );
}

template< typename TYPES >
template< typename tResponseData >
const tResponseData* TProxy< TYPES >::get_event_data( const typename TYPES::tEvent& event )
{
   return static_cast< tResponseData* >( event.data( )->ptr.get( ) );
}



} // namespace base



#undef CLASS_ABBR
