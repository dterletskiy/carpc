#pragma once

#include "api/sys/application/Process.hpp"
#include "api/sys/comm/service/IProxy.hpp"
#include "api/sys/comm/service/experimental/TClient.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TProxyExp"



namespace carpc::service::experimental::__private__ {

   template< typename TYPES >
      class TServer;
   template< typename TYPES >
      class TProxy;
   template< typename TYPES >
      class TClient;

} // namespace carpc::service::experimental::__private__



namespace carpc::service::experimental::__private_proxy__ {

   template< typename TYPES >
   class MethodProcessor
   {
      using tProxy = __private__::TProxy< TYPES >;
      using tClient = __private__::TClient< TYPES >;

      using tSeqIDClientMap = std::map< comm::sequence::ID, tClient* >;
      using tRequestMap = std::map< typename TYPES::method::tEventID, tSeqIDClientMap >;

      public:
         MethodProcessor( tProxy* );
         void reset( );

      public:
         template< typename tRequestData, typename... Args >
            const comm::sequence::ID request( tClient* p_client, const Args&... args );
         const bool process( const typename TYPES::method::tEvent& );

      private:
         comm::sequence::ID   m_seq_id = comm::sequence::ID::zero;
         tRequestMap          m_map;
         tProxy*              mp_proxy = nullptr;
   };



   template< typename TYPES >
   MethodProcessor< TYPES >::MethodProcessor( tProxy* p_proxy )
      : mp_proxy( p_proxy )
   {
   }

   template< typename TYPES >
   void MethodProcessor< TYPES >::reset( )
   {
      for( auto item : m_map )
         item.second.clear( );
   }

   template< typename TYPES >
   template< typename tRequestData, typename... Args >
   const comm::sequence::ID MethodProcessor< TYPES >::request( tClient* p_client, const Args&... args )
   {
       auto event_id_iterator = m_map.emplace( tRequestData::ID, tSeqIDClientMap{ } ).first;

      // If response exists for current request
      // if( true == tRequestData::IS_RESPONSE )
      if( true == TYPES::T::method::has_response( tRequestData::ID ) )
      {
         auto& client_map = event_id_iterator->second;

         // Incrementing common sequence id for all requests in this proxy. This sequence id will be sent to server in event.
         // Also new sequence id and corresponding client pointer are added to the map for later client identifying in response
         // by sequence id received from server.
         auto result = client_map.emplace( ++m_seq_id, p_client );
         if( false == result.second )
         {
            SYS_WRN( "emplace error: %s -> %p", m_seq_id.name( ).c_str( ), p_client );
            return comm::sequence::ID::invalid;
         }

         // In case if client map contains only one element this means that this is just added client
         // and current proxy was not subscribed for corresponding responses of mentioned request.
         if( 1 == client_map.size( ) )
         {
            TYPES::method::tEvent::set_notification(
               mp_proxy,
               typename TYPES::method::tEventUserSignature(
                  mp_proxy->signature( ).role( ),
                  tRequestData::ID,
                  carpc::service::eType::RESPONSE,
                  mp_proxy->server( ).id( ),
                  mp_proxy->id( )
               )
            );
            TYPES::method::tEvent::set_notification(
               mp_proxy,
               typename TYPES::method::tEventUserSignature(
                  mp_proxy->signature( ).role( ),
                  tRequestData::ID,
                  carpc::service::eType::BUSY,
                  mp_proxy->server( ).id( ),
                  mp_proxy->id( )
               )
            );
         }
      }

      // Preparing and sending request event
      typename TYPES::method::tEventUserSignature event_signature(
            mp_proxy->signature( ).role( ),
            tRequestData::ID,
            carpc::service::eType::REQUEST,
            mp_proxy->id( ),
            mp_proxy->server( ).id( ),
            m_seq_id
         );
      typename TYPES::method::tEventData data( std::make_shared< tRequestData >( args... ) );
      TYPES::method::tEvent::create_send( event_signature, data, mp_proxy->server( ).context( ) );

      return m_seq_id;
   }

   template< typename TYPES >
   const bool MethodProcessor< TYPES >::process( const typename TYPES::method::tEvent& event )
   {
      const typename TYPES::method::tEventID event_id = event.info( ).id( );
      const comm::sequence::ID seq_id = event.info( ).seq_id( );

      auto event_id_iterator = m_map.find( event_id );
      if( m_map.end( ) == event_id_iterator )
      {
         // This means that event with current id was not send before by this proxy.
         SYS_WRN( "method id was not found: %s", event_id.c_str( ) );
         return false;
      }

      auto& client_map = event_id_iterator->second;

      // In case if client map for current request contains only one client this client
      // will be removed at the end of current scope and client map will pe empty.
      // This means current proxy is not interested in following response notifications
      // related to current request.
      // This also makes sence only in case if response is defined for current method.
      if( 1 == client_map.size( ) && true == TYPES::T::method::has_response( event_id ) )
      {
         TYPES::method::tEvent::clear_notification(
               mp_proxy,
               typename TYPES::method::tEventUserSignature(
                  mp_proxy->signature( ).role( ),
                  event_id,
                  carpc::service::eType::RESPONSE,
                  mp_proxy->server( ).id( ),
                  mp_proxy->id( )
               )
            );
         TYPES::method::tEvent::clear_notification(
               mp_proxy,
               typename TYPES::method::tEventUserSignature(
                  mp_proxy->signature( ).role( ),
                  event_id,
                  carpc::service::eType::BUSY,
                  mp_proxy->server( ).id( ),
                  mp_proxy->id( )
               )
            );
      }

      auto seq_id_iterator = client_map.find( seq_id );
      if( client_map.end( ) == seq_id_iterator )
      {
         SYS_WRN( "delivered event with sequence id '%s' to unknown client", m_seq_id.name( ).c_str( ) );
         return false;
      }
      seq_id_iterator->second->process_event( event );
      client_map.erase( seq_id_iterator );

      return true;
   }

} // namespace carpc::service::experimental::__private_proxy__



namespace carpc::service::experimental::__private_proxy__ {

   template< typename TYPES >
   struct AttributeDB
   {
      using tClient = __private__::TClient< TYPES >;
      using tClientsSet = std::set< tClient* >;

      tClientsSet                                                 m_client_set;
      std::shared_ptr< typename TYPES::attribute::tEventData >    mp_event_data = nullptr;
   };



   template< typename TYPES >
   class AttributeProcessor
   {
      using tProxy = __private__::TProxy< TYPES >;
      using tClient = __private__::TClient< TYPES >;
      using tClientsSet = std::set< tClient* >;
      using tAttributeDB = AttributeDB< TYPES >;

      public:
         AttributeProcessor( tProxy* );
         void reset( );

      public:
         template< typename tAttributeData >
            const bool subscribe( tClient* );
         template< typename tAttributeData >
            const bool unsubscribe( tClient* );
         const bool process( const typename TYPES::attribute::tEvent& );

      private:
         std::map< typename TYPES::attribute::tEventID, tAttributeDB >  m_map;
         tProxy*                                                        mp_proxy = nullptr;
   };



   template< typename TYPES >
   AttributeProcessor< TYPES >::AttributeProcessor( tProxy* p_proxy )
      : mp_proxy( p_proxy )
   {
   }

   template< typename TYPES >
   void AttributeProcessor< TYPES >::reset( )
   {
      for( auto item : m_map )
         item.second = tAttributeDB{ };
   }

   template< typename TYPES >
   template< typename tAttributeData >
   const bool AttributeProcessor< TYPES >::subscribe( tClient* p_client )
   {
      auto event_id_iterator = m_map.emplace( tAttributeData::ID, tAttributeDB{ } ).first;

      // In clients_set is empty this means current proxy is not subscribed
      // for corresponding attribute notification
      tClientsSet& clients_set = event_id_iterator->second.m_client_set;
      if( clients_set.empty( ) )
      {
         TYPES::attribute::tEvent::set_notification(
               mp_proxy,
               typename TYPES::attribute::tEventUserSignature(
                  mp_proxy->signature( ).role( ),
                  tAttributeData::ID,
                  carpc::service::eType::NOTIFICATION,
                  mp_proxy->server( ).id( ),
                  mp_proxy->id( )
               )
            );
         TYPES::attribute::tEvent::create_send(
               typename TYPES::attribute::tEventUserSignature(
                  mp_proxy->signature( ).role( ),
                  tAttributeData::ID,
                  carpc::service::eType::SUBSCRIBE,
                  mp_proxy->id( ),
                  mp_proxy->server( ).id( )
               ),
               mp_proxy->server( ).context( )
            );
      }
      clients_set.emplace( p_client );

      if( nullptr != event_id_iterator->second.mp_event_data )
      {
         SYS_VRB( "having cached attribute event" );

         auto p_event = TYPES::attribute::tEvent::create(
               typename TYPES::attribute::tEventUserSignature(
                     mp_proxy->signature( ).role( ),
                     tAttributeData::ID,
                     carpc::service::eType::NOTIFICATION
                  )
            );
         p_event->data( event_id_iterator->second.mp_event_data );
         auto operation = [ p_client, p_event ]( ){ p_client->process_event( *p_event ); };
         carpc::async::Runnable::create_send( operation );
      }

      return true;
   }

   template< typename TYPES >
   template< typename tAttributeData >
   const bool AttributeProcessor< TYPES >::unsubscribe( tClient* p_client )
   {
      auto event_id_iterator = m_map.find( tAttributeData::ID );
      if( m_map.end( ) == event_id_iterator )
      {
         SYS_WRN( "notification id does not exist: %s", tAttributeData::ID.c_str( ) );
         return false;
      }

      tClientsSet& clients_set = event_id_iterator->second.m_client_set;
      clients_set.erase( p_client );
      if( clients_set.empty( ) )
      {
         TYPES::attribute::tEvent::clear_notification(
            mp_proxy,
            typename TYPES::attribute::tEventUserSignature(
                  mp_proxy->signature( ).role( ),
                  tAttributeData::ID,
                  carpc::service::eType::NOTIFICATION,
                  mp_proxy->server( ).id( ),
                  mp_proxy->id( )
               )
         );
         TYPES::attribute::tEvent::create_send(
            typename TYPES::attribute::tEventUserSignature(
               mp_proxy->signature( ).role( ),
               tAttributeData::ID,
               carpc::service::eType::UNSUBSCRIBE,
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
   const bool AttributeProcessor< TYPES >::process( const typename TYPES::attribute::tEvent& event )
   {
      const typename TYPES::attribute::tEventID event_id = event.info( ).id( );

      auto event_id_iterator = m_map.find( event_id );
      if( m_map.end( ) == event_id_iterator )
      {
         SYS_WRN( "notification id does not exist: %s", event_id.c_str( ) );
         return false;
      }

      tAttributeDB& attribute_db = event_id_iterator->second;

      attribute_db.mp_event_data = event.data( );

      for( auto p_client : attribute_db.m_client_set )
         p_client->process_event( event );

      return true;
   }

} // namespace carpc::service::experimental::__private_proxy__



namespace carpc::service::experimental::__private__ {

   template< typename TYPES >
   class TProxy
      : public IProxy
      , public TYPES::method::tEventConsumer
      , public TYPES::attribute::tEventConsumer
   {
      using tClient = TClient< TYPES >;
      using tProxy = TProxy< TYPES >;
      using tMethodProcessor = __private_proxy__::MethodProcessor< TYPES >;
      using tAttributeProcessor = __private_proxy__::AttributeProcessor< TYPES >;

      private:
         TProxy( const carpc::async::tAsyncTypeID&, const std::string&, const bool );
         static thread_local tProxy* sp_proxy;
      public:
         ~TProxy( ) override;
         static tProxy* create( const carpc::async::tAsyncTypeID&, const std::string&, const bool );

      private:
         void connected( const Address& ) override final;
         void disconnected( const Address& ) override final;
         void connected( ) override final { }
         void disconnected( ) override final { }

      private:
         void process_event( const typename TYPES::method::tEvent& ) override final;
         void process_event( const typename TYPES::attribute::tEvent& ) override final;

      public:
         template< typename tRequestData, typename... Args >
            const comm::sequence::ID request( tClient*, const Args&... args );
         template< typename tAttributeData >
            const bool subscribe( tClient* );
         template< typename tAttributeData >
            const bool unsubscribe( tClient* );
         template< typename tMethodData >
            const tMethodData* get_event_data( const typename TYPES::method::tEvent& event );
         template< typename tAttributeData >
            const tAttributeData* get_event_data( const typename TYPES::attribute::tEvent& event );

      private:
         tMethodProcessor     m_method_processor;
         tAttributeProcessor  m_attribute_processor;
   };



   template< typename TYPES >
   TProxy< TYPES >::TProxy( const carpc::async::tAsyncTypeID& interface_type_id, const std::string& role_name, const bool is_import )
      : IProxy( interface_type_id, role_name, is_import )
      , TYPES::method::tEventConsumer( )
      , TYPES::attribute::tEventConsumer( )
      , m_method_processor( this )
      , m_attribute_processor( this )
   {
   }

   template< typename TYPES >
   TProxy< TYPES >::~TProxy( )
   {
      TYPES::method::tEvent::clear_all_notifications( this );
      TYPES::attribute::tEvent::clear_all_notifications( this );
   }

   template< typename TYPES >
   thread_local TProxy< TYPES >* TProxy< TYPES >::sp_proxy = nullptr;

   template< typename TYPES >
   TProxy< TYPES >* TProxy< TYPES >::create( const carpc::async::tAsyncTypeID& interface_type_id, const std::string& role_name, const bool is_import )
   {
      if( nullptr != sp_proxy )
      {
         SYS_VRB( "return existing proxy: %p", sp_proxy );
         return sp_proxy;
      }

      if( application::thread::current_id( ).is_invalid( ) )
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
      SYS_VRB( "proxy created: %p", sp_proxy );

      return sp_proxy;
   }

   template< typename TYPES >
   void TProxy< TYPES >::connected( const Address& server_address )
   {
      m_method_processor.reset( );
      m_attribute_processor.reset( );
   }

   template< typename TYPES >
   void TProxy< TYPES >::disconnected( const Address& server_address )
   {
      m_method_processor.reset( );
      m_attribute_processor.reset( );
   }

   template< typename TYPES >
   void TProxy< TYPES >::process_event( const typename TYPES::method::tEvent& event )
   {
      SYS_VRB( "processing event: %s", event.info( ).name( ).c_str( ) );
      m_method_processor.process( event );
   }

   template< typename TYPES >
   void TProxy< TYPES >::process_event( const typename TYPES::attribute::tEvent& event )
   {
      SYS_VRB( "processing event: %s", event.info( ).name( ).c_str( ) );
      m_attribute_processor.process( event );
   }

   template< typename TYPES >
   template< typename tRequestData, typename... Args >
   const comm::sequence::ID TProxy< TYPES >::request( tClient* p_client, const Args&... args )
   {
      if( !is_connected( ) )
      {
         SYS_WRN( "proxy is not connected" );
         return comm::sequence::ID::invalid;
      }

      return m_method_processor.template request< tRequestData >( p_client, args... );
   }

   template< typename TYPES >
   template< typename tAttributeData >
   const bool TProxy< TYPES >::subscribe( tClient* p_client )
   {
      if( !is_connected( ) )
      {
         SYS_WRN( "proxy is not connected" );
         return false;
      }

      return m_attribute_processor.template subscribe< tAttributeData >( p_client );
   }

   template< typename TYPES >
   template< typename tAttributeData >
   const bool TProxy< TYPES >::unsubscribe( tClient* p_client )
   {
      return m_attribute_processor.template unsubscribe< tAttributeData >( p_client );
   }

   template< typename TYPES >
   template< typename tMethodData >
   const tMethodData* TProxy< TYPES >::get_event_data( const typename TYPES::method::tEvent& event )
   {
      if( const tMethodData* p_data = static_cast< tMethodData* >( event.data( )->ptr.get( ) ) )
         return p_data;

      SYS_ERR( "missing data for method ID: %s", event.info( ).id( ).c_str( ) );
      return nullptr;
   }

   template< typename TYPES >
   template< typename tAttributeData >
   const tAttributeData* TProxy< TYPES >::get_event_data( const typename TYPES::attribute::tEvent& event )
   {
      if( const tAttributeData* p_data = static_cast< tAttributeData* >( event.data( )->ptr.get( ) ) )
         return p_data;

      SYS_ERR( "missing data for attribute ID: %s", event.info( ).id( ).c_str( ) );
      return nullptr;
   }

} // namespace carpc::service::experimental::__private__



#undef CLASS_ABBR
