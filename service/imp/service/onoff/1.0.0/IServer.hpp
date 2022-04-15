#pragma once

#include "api/sys/comm/service/fast/TServer.hpp"
#include "api/sys/comm/service/secure/TServer.hpp"
#include "Data.hpp"



namespace service::onoff::V1_0_0 {

   class IServer
      : public carpc::service::SERVICE_NAMESPACE::TServer< data::Types >
   {
      public:
         IServer( const std::string& );
         virtual ~IServer( ) = default;

      private:
         void process_request_event( const tService::Event& ) override final;

      public:
         virtual void request_start( ) = 0;
         virtual void request_trigger_state( const std::string&, const std::size_t ) = 0;
         void response_trigger_state( const bool );

      public:
         void current_state( const std::string& );
         const std::string& current_state( ) const;
   };

} // namespace service::onoff::V1_0_0
