#pragma once

#include "imp/experimental/base/component/Base.hpp"
#include "imp/experimental/api/events/OnOff.hpp"



namespace component::onoff::servers {

   class OnOff;

}



namespace component::onoff {

   class Component : public fw::component::Base
   {
      public:
         static std::unique_ptr< fw::component::Base > creator( );

      public:
         Component( );
         ~Component( ) override;

      private:
         std::shared_ptr< servers::OnOff > mp_onoff_server;
   };

} // namespace component::onoff
