#pragma once

#include "api/sys/application/IComponent.hpp"



namespace base::application {

   class RootComponent : public IComponent
   {
      public:
         RootComponent( const std::string& );
         ~RootComponent( ) override;

      private:
         const bool is_root( ) const override { return true; }
      protected:
         void shutdown( const std::string& info = "shutdown" ) override final;
   };

} // namespace base::application
