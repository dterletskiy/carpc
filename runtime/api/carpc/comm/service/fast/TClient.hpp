#pragma once

#include "carpc/comm/service/IClient.hpp"
#include "carpc/comm/service/fast/TProxy.hpp"
#include "carpc/comm/service/fast/TGenerator.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "TClientFast"



namespace carpc::service::fast::__private__ {

   template< typename TYPES >
   class TClient : public IClient
   {
      using tProxy = TProxy< TYPES >;
      using tClient = TClient< TYPES >;

      public:
         TClient( const carpc::async::tAsyncTypeID&, const std::string&, const bool );
         ~TClient( ) override;

      public:
         virtual void process_response_event( const typename TYPES::tEvent& ) = 0;
         virtual void process_notification_event( const typename TYPES::tEvent& ) = 0;

      protected:
         template< typename tRequestData, typename... Args >
            const comm::sequence::ID request( tClient*, const Args&... );
         template< typename tNotificationData >
            const bool subscribe( tClient* p_client );
         template< typename tNotificationData >
            const bool unsubscribe( tClient* p_client );
         template< typename tResponseData >
            const tResponseData* get_event_data( const typename TYPES::tEvent& );

      private:
         tProxy* proxy( ) const;
         tProxy* mp_proxy = nullptr;
   };



   template< typename TYPES >
   TClient< TYPES >::TClient( const carpc::async::tAsyncTypeID& interface_type_id, const std::string& role_name, const bool is_import )
   {
      mp_proxy = tProxy::create( interface_type_id, role_name, is_import );
      mp_proxy->register_client( this );
      if( mp_proxy )
      {
         SYS_VRB( "created: %s", mp_proxy->signature( ).dbg_name( ).c_str( ) );
      }
      else
      {
         SYS_ERR( "proxy can't be created => client will not work: %s / %s / %s",
                     interface_type_id.dbg_name( ).c_str( ),
                     role_name.c_str( ),
                     is_import ? "import" : " no import"
            );
      }
   }

   template< typename TYPES >
   TClient< TYPES >::~TClient( )
   {
      if( mp_proxy )
      {
         SYS_VRB( "destroyed: %s", mp_proxy->signature( ).dbg_name( ).c_str( ) );
         mp_proxy->unregister_client( this );
         mp_proxy = nullptr;
      }
   }

   template< typename TYPES >
   template< typename tRequestData, typename... Args >
   const comm::sequence::ID TClient< TYPES >::request( tClient* p_client, const Args&... args )
   {
      return mp_proxy->template request< tRequestData >( this, args... );
   }

   template< typename TYPES >
   template< typename tNotificationData >
   const bool TClient< TYPES >::subscribe( tClient* p_client )
   {
      return mp_proxy->template subscribe< tNotificationData >( this );
   }

   template< typename TYPES >
   template< typename tNotificationData >
   const bool TClient< TYPES >::unsubscribe( tClient* p_client )
   {
      return mp_proxy->template unsubscribe< tNotificationData >( this );
   }

   template< typename TYPES >
   template< typename tResponseData >
   const tResponseData* TClient< TYPES >::get_event_data( const typename TYPES::tEvent& event )
   {
      if( const tResponseData* p_data = mp_proxy->template get_event_data< tResponseData >( event ) )
         return p_data;

      SYS_ERR( "missing data for response/notification ID: %s", event.info( ).id( ).c_str( ) );
      return nullptr;
   }

   template< typename TYPES >
   typename TClient< TYPES >::tProxy* TClient< TYPES >::proxy( ) const
   {
      return mp_proxy;
   }

} // namespace carpc::service::fast::__private__



namespace carpc::service::fast {

   template< typename TYPES >
   class TClient : public __private__::TClient< TGenerator< TYPES > >
   {
      public:
         TClient( const std::string&, const bool );

         using tService = typename TGenerator< TYPES >::Service;
   };

   template< typename TYPES >
   TClient< TYPES >::TClient( const std::string& role_name, const bool is_export )
      : __private__::TClient< TGenerator< TYPES > >( TGenerator< TYPES >::interface_type_id, role_name, is_export )
   {
      REGISTER_EVENT( tService );
   }

} // namespace carpc::service::fast



#undef CLASS_ABBR
