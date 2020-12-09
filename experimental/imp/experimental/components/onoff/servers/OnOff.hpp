#pragma once

#include "imp/experimental/api/onoff/Stub.hpp"



namespace component::onoff::servers {

   class OnOff : public api::onoff::Stub
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
         void request_boot( const std::string& ) override;
   };

} // namespace component::onoff::servers
