#pragma once

#include "api/sys/comm/service/fast/TServer.hpp"
#include "api/sys/comm/service/secure/TServer.hpp"
#include "api/app/services/onoff/1.0/Data.hpp"



namespace service::onoff::V1_0_0 {

   class Server
      : public carpc::service::SERVICE_NAMESPACE::TServer< data::Types >
   {
      public:
         using tSptr = std::shared_ptr< Server >;

      public:
         Server( const std::string& );
         virtual ~Server( );

      private:
         void connected( ) override;
         void disconnected( ) override;

      private:
         void process_request_event( const tService::Event& ) override;

      public:
         virtual void request_trigger_state( const std::string&, const std::size_t ) = 0;
         void response_trigger_state( const bool );

      public:
         void current_state( const std::string& );
         const std::string& current_state( ) const;
   };

} // namespace service::onoff::V1_0_0
