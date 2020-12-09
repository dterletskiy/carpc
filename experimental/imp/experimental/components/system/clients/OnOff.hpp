#pragma once

#include "imp/experimental/api/onoff/Proxy.hpp"



namespace component::system::clients {

   class OnOff : public api::onoff::Proxy
   {
      private:
         OnOff( );
      public:
         ~OnOff( ) override;
         static std::shared_ptr< OnOff > create( );

      private:
         void connected( ) override;
         void disconnected( ) override;

      public:
         void response_boot( const bool ) override;
   };

} // namespace component::system::clients
