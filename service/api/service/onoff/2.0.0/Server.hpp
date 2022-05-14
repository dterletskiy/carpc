#pragma once

#include "Version.hpp"

namespace service::onoff::V2_0_0 {

   class ServerImpl;

   class Server
   {
         friend class ServerImpl;

      public:
         using tSptr = std::shared_ptr< Server >;

      public:
         Server( const std::string& role_name );
         virtual ~Server( );

      private:
         virtual void connected( ) = 0;
         virtual void disconnected( ) = 0;

      public:
         virtual void request_trigger_state( const std::string& _state, const std::size_t& _timeout ) = 0;
         void response_trigger_state( const bool& _result );

         virtual void request_start( ) = 0;


      public:
         void current_state( const std::string& );
         const std::string& current_state( ) const;

      public:
         const carpc::comm::sequence::ID unblock_request( );
         void prepare_response( const carpc::comm::sequence::ID& );

      private:
         ServerImpl* mp_impl = nullptr;
   };

} // namespace service::onoff::V2_0_0


