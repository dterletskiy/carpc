#pragma once

#include <string>

#include "Version.hpp"



namespace service::onoff::V2_0_0 {

   class ClientImpl;

   class Client
   {
         friend class ClientImpl;

      public:
         using tSptr = std::shared_ptr< Client >;

      public:
         Client( const std::string& );
         ~Client( );

      private:
         virtual void connected( ) = 0;
         virtual void disconnected( ) = 0;

      private:
         virtual void response_trigger_state( const bool ) = 0;
         virtual void request_trigger_state_failed( const carpc::service::eError& ) = 0;
         virtual void on_current_state( const std::string& ) = 0;

      public:
         void request_start( );
         void request_trigger_state( const std::string&, const size_t );
         void subscribe_current_state( );
         void unsubscribe_current_state( );

      private:
         ClientImpl* mp_impl = nullptr;
   };

} // namespace service::onoff::V2_0_0
