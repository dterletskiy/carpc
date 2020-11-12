#pragma once

#include "imp/experimental/base/service/IService.hpp"



namespace fw::service {

   class IStub;

   class IProxy : public IService
   {
      public:
         IProxy( );
         ~IProxy( ) override;

         void connected( std::shared_ptr< IStub > );
         void disconnected( std::shared_ptr< IStub > );

         bool is_connected( ) const;
         bool is_disconnected( ) const;

      private:
         std::weak_ptr< IStub > mp_stub;
   };

} // namespace fw::service
