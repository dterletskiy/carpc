#pragma once

#include "api/sys/comm/service/experimental/TServer.hpp"
#include "Data.hpp"

namespace service::onoff::V2_0_0 {

   class IServer
      : public carpc::service::experimental::TServer< Types >
   {
      public:
         IServer( const std::string& role_name );
         ~IServer( ) override = default;

      private:
         void process_request_event( const tMethod::Event& event ) override;

      private:
         virtual void request_trigger_state( const std::string& _state, const std::size_t& _timeout ) = 0;
      public:
         void response_trigger_state( const bool& _result );

      private:
         virtual void request_start( ) = 0;


      public:
         void current_state( const std::string& );
         const std::string& current_state( ) const;
   };

} // namespace service::onoff::V2_0_0


