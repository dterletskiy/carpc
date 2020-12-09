#pragma once

#include "imp/experimental/base/service/IStub.hpp"



namespace api::onoff {

   class EventConsumerStub;



   class Stub : public fw::service::IStub
   {
      public:
         Stub( const fw::service::tServiceRole& );
         ~Stub( ) override;

      public:
         virtual void request_boot( const std::string& ) = 0;

      public:
         void response_boot( const bool );

      private:
         std::shared_ptr< EventConsumerStub > mp_event_consumer = nullptr;
   };

} // namespace fw::api::onoff
