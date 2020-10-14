#pragma once

#include "api/sys/comm/service/IClient.hpp"
#include "api/sys/comm/service/secure/TProxy.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TClientSecure"



namespace base::service::secure {

   template< typename TYPES >
   class TClient : public IClient
   {
      using tProxy = TProxy< TYPES >;

      public:
         TClient( const base::async::tAsyncTypeID&, const std::string&, const bool );
         ~TClient( ) override;

      public:
         virtual void process_response_event( const typename TYPES::tEvent& ) = 0;
         virtual void process_notification_event( const typename TYPES::tEvent& ) = 0;

      protected:
         tProxy* mp_proxy = nullptr;
   };



   template< typename TYPES >
   TClient< TYPES >::TClient( const base::async::tAsyncTypeID& interface_type_id, const std::string& role_name, const bool is_import )
   {
      mp_proxy = tProxy::create( interface_type_id, role_name, is_import );
      mp_proxy->register_client( this );
   }

   template< typename TYPES >
   TClient< TYPES >::~TClient( )
   {
      mp_proxy->unregister_client( this );
      mp_proxy = nullptr;
   }

} // namespace base::service::secure



#undef CLASS_ABBR
