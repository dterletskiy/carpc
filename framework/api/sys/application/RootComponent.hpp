#pragma once

#include "api/sys/application/IComponent.hpp"



namespace carpc::application {

   class RootComponent : public IComponent
   {
      public:
         RootComponent( const std::string& );
         ~RootComponent( ) override;

      private:
         const bool is_root( ) const override { return true; }
      protected:
         void shutdown( const std::string& info = "shutdown" );
   };

} // namespace carpc::application
