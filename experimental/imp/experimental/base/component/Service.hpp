#pragma once

#include "imp/experimental/base/component/Base.hpp"



namespace fw::component {

   class Service : public Base
   {
      public:
         static std::unique_ptr< Base > creator( );

      public:
         Service( );
         ~Service( ) override;
   };

}
