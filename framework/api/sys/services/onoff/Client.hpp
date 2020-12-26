#pragma once

#include "api/sys/comm/service/fast/TClient.hpp"
#include "api/sys/comm/service/secure/TClient.hpp"
#include "api/sys/services/onoff/Data.hpp"



namespace base::onoff {

   class Client
      : public base::service::SERVICE_NAMESPACE::TClient< data::Types >
   {
      public:
         using tSptr = std::shared_ptr< Client >;

      public:
         Client( const std::string& );
         virtual ~Client( );

      private:
         void connected( ) override;
         void disconnected( ) override;

      private:
         void process_response_event( const tService::Event& ) override;
         void process_notification_event( const tService::Event& ) override;

      public:
         void request_start( );
         const base::service::tSequenceID request_trigger_state( const std::string&, const std::size_t );
         void subscribe_current_state( );
         void unsubscribe_current_state( );
      private:
         virtual void request_trigger_state_failed( ) = 0;
         virtual void response_trigger_state( const bool ) = 0;
         virtual void on_current_state( const std::string& ) = 0;
   };

} // namespace base::onoff
