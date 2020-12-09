#pragma once

#include "imp/experimental/base/component/Base.hpp"
#include "imp/experimental/api/events/OnOff.hpp"



namespace component::system::clients {

   class OnOff;

}



namespace component::system {

   struct OnOffEventConsumer : public events::IOnOffEventConsumer
   {
      void process( const events::OnOffEvent& event ) override;
   };



   class Component : public fw::component::Base
   {
      public:
         static std::unique_ptr< fw::component::Base > creator( );

      public:
         Component( );
         ~Component( ) override;

      private:
         std::shared_ptr< clients::OnOff > mp_onoff_client;

      private:
         std::shared_ptr< OnOffEventConsumer > mp_onoff_event_consumer;
   };

}
