#pragma once

#include "api/sys/comm/service/fast/TServer.hpp"
#include "api/sys/comm/service/secure/TServer.hpp"
#include "api/sys/onoff/Data.hpp"



namespace base::onoff {

   class Server
      : public base::service::SERVICE_NAMESPACE::TServer< data::Types >
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
         void process_request_event( const data::OnOffEvent::Event& ) override;

      public:
         virtual void request_start( ) = 0;
         virtual void request_trigger_state( const std::string&, const size_t ) = 0;
         void response_trigger_state( const bool );

      public:
         void current_state( const std::string& );
         const std::string& current_state( ) const;
   };

} // namespace base::onoff
