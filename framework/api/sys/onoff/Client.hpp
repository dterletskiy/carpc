#pragma once

#include "api/sys/comm/interface/TClient.hpp"
#include "api/sys/onoff/Data.hpp"



namespace base::onoff {

   class Client
      : public base::interface::TClient< data::Types >
   {
      public:
         Client( const std::string& );
         virtual ~Client( );

      private:
         void connected( ) override;
         void disconnected( ) override;

      private:
         void process_response_event( const data::OnOffEvent::Event& ) override;
         void process_notification_event( const data::OnOffEvent::Event& ) override;

      public:
         void request_start( );
         const size_t request_trigger_state( const std::string&, const size_t );
         void subscribe_current_state( );
         void unsubscribe_current_state( );
      private:
         virtual void request_trigger_state_failed( ) = 0;
         virtual void response_trigger_state( const bool ) = 0;
         virtual void on_current_state( const std::string& ) = 0;
   };

} // namespace base::onoff
