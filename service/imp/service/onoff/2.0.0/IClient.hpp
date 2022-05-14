#pragma once

#include "api/sys/comm/service/experimental/TClient.hpp"
#include "Data.hpp"

namespace service::onoff::V2_0_0 {

   class IClient
      : public carpc::service::experimental::TClient< Types >
   {
      public:
         IClient( const std::string& role_name );
         ~IClient( ) override = default;

      private:
         void process_event( const tMethod::Event& event ) override;
         void process_event( const tAttribute::Event& event ) override;

      public:
         carpc::comm::sequence::ID request_trigger_state( const std::string& state, const std::size_t& timeout );
      private:
         virtual void response_trigger_state( const bool& _result ) = 0;
         virtual void request_trigger_state_failed( const carpc::service::eError& error ) = 0;

      public:
         void request_start( );


      public:
         void subscribe_current_state( );
         void unsubscribe_current_state( );
      private:
         virtual void on_current_state( const std::string& ) = 0;
   };

} // namespace service::onoff::V2_0_0


