#pragma once

#include "api/sys/comm/interface/IClient.hpp"
#include "api/sys/comm/interface/TProxy.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TClient"



namespace base {



   template< typename TYPES >
   class TClient : public IClient
   {
      using tProxy = TProxy< TYPES >;

      public:
         TClient( const std::string&, const std::string& );
         ~TClient( ) override;

      public:
         virtual void process_response_event( const typename TYPES::tEvent& ) = 0;
         virtual void process_notification_event( const typename TYPES::tEvent& ) = 0;

      protected:
         tProxy* mp_proxy = nullptr;
   };



   template< typename TYPES >
   TClient< TYPES >::TClient( const std::string& interface_name, const std::string& role_name )
   {
      mp_proxy = tProxy::create( interface_name, role_name );
      mp_proxy->register_client( this );
   }

   template< typename TYPES >
   TClient< TYPES >::~TClient( )
   {
      mp_proxy->unregister_client( this );
      mp_proxy = nullptr;
   }


} // namespace base



#undef CLASS_ABBR
