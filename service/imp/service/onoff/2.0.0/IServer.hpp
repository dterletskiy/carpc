#pragma once

#include "api/sys/comm/service/experimental/TServer.hpp"
#include "Data.hpp"



namespace service::onoff::V2_0_0 {

   class IServer
      : public carpc::service::experimental::TServer< data::Types >
   {
      public:
         IServer( const std::string& );
         ~IServer( ) override = default;

      private:
         void process_request_event( const tMethod::Event& ) override;

      public:
         void response_trigger_state( const bool );
      private:
         virtual void request_start( ) = 0;
         virtual void request_trigger_state( const std::string&, const std::size_t ) = 0;

      public:
         void current_state( const std::string& );
         const std::string& current_state( ) const;
   };

} // namespace service::onoff::V2_0_0
