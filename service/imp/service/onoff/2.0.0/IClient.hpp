#pragma once

#include "api/sys/comm/service/experimental/TClient.hpp"
#include "Data.hpp"



namespace service::onoff::V2_0_0 {

   class IClient
      : public carpc::service::experimental::TClient< data::Types >
   {
      public:
         IClient( const std::string& );
         virtual ~IClient( ) = default;

      private:
         void process_event( const tMethod::Event& ) override;
         void process_event( const tAttribute::Event& ) override;

      public:
         const carpc::comm::sequence::ID request_trigger_state( const std::string&, const std::size_t );
      private:
         virtual void request_trigger_state_failed( const carpc::service::eError& ) = 0;
         virtual void response_trigger_state( const bool ) = 0;

      public:
         void request_start( );

      public:
         void subscribe_current_state( );
         void unsubscribe_current_state( );
      private:
         virtual void on_current_state( const std::string& ) = 0;
   };

} // namespace service::onoff::V2_0_0
