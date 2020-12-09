#pragma once

#include "imp/experimental/base/service/IProxy.hpp"



namespace api::onoff {

   class EventConsumerProxy;



   class Proxy : public fw::service::IProxy
   {
      private:

      public:
         Proxy( const fw::service::tServiceRole& );
         ~Proxy( ) override;

      public:
         virtual void response_boot( const bool ) = 0;

      public:
         void request_boot( const std::string& );

      private:
         std::shared_ptr< EventConsumerProxy > mp_event_consumer = nullptr;
   };

} // namespace api::onoff
