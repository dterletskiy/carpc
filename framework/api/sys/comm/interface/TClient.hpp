#pragma once

#include "api/sys/comm/interface/IClient.hpp"
#include "api/sys/comm/interface/TProxy.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TClient"



namespace base::interface {



   template< typename TYPES >
   class TClient : public IClient
   {
      using tProxy = TProxy< TYPES >;

      public:
         TClient( const tAsyncTypeID&, const std::string&, const bool );
         ~TClient( ) override;

      public:
         virtual void process_response_event( const typename TYPES::tEvent& ) = 0;
         virtual void process_notification_event( const typename TYPES::tEvent& ) = 0;

      protected:
         tProxy* mp_proxy = nullptr;
   };



   template< typename TYPES >
   TClient< TYPES >::TClient( const tAsyncTypeID& interface_type_id, const std::string& role_name, const bool is_import )
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


} // namespace base::interface



#undef CLASS_ABBR
