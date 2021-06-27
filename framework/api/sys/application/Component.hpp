#pragma once

#include "api/sys/application/IComponent.hpp"



namespace base::application {

   class Component : public IComponent
   {
      public:
         Component( const std::string& );
         ~Component( ) override;

      private:
         const bool is_root( ) const override { return false; }
         void boot( const std::string& info = "boot" ) override final { }
         void shutdown( const std::string& info = "shutdown" ) override final { }
   };

} // namespace base::application
