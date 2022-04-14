#pragma once

#include "api/sys/comm/service/experimental/TServer.hpp"
#include "api/sys/services/onoff_exp/Data.hpp"



namespace service::onoff {

   class Server
      : public carpc::service::experimental::TServer< data::Types >
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
         void process_request_event( const tMethod::Event& ) override;

      public:
         virtual void request_start( ) = 0;
         virtual void request_trigger_state( const std::string&, const std::size_t ) = 0;
         void response_trigger_state( const bool );

      public:
         void current_state( const std::string& );
         const std::string& current_state( ) const;
   };

} // namespace service::onoff
