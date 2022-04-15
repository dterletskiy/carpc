#pragma once

#include <string>

#include "Version.hpp"



namespace service::onoff::V2_0_0 {

   class ServerImpl;

   class Server
   {
         friend class ServerImpl;

      public:
         using tSptr = std::shared_ptr< Server >;

      public:
         Server( const std::string& );
         virtual ~Server( );

      private:
         virtual void connected( ) = 0;
         virtual void disconnected( ) = 0;

      public:
         virtual void request_start( ) = 0;
         virtual void request_trigger_state( const std::string&, const size_t ) = 0;

      public:
         void response_trigger_state( const bool );
         void current_state( const std::string& );
         const std::string& current_state( ) const;

      public:
         const carpc::comm::sequence::ID unblock_request( );
         void prepare_response( const carpc::comm::sequence::ID& );

      private:
         ServerImpl* mp_impl = nullptr;
   };

} // namespace service::onoff::V2_0_0
