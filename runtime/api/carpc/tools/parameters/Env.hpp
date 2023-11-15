#pragma once

#include "carpc/tools/parameters/Base.hpp"



namespace carpc::tools::parameters {

   class Env: public Base
   {
      public:
         // Argument name must be split with it's value by this delimiter
         static const tValue default_prefix;
         // Each argument name must start with this prefix
         static const tValue default_delimiter;

      private:
         Env( const tMap& map );
      public:
         ~Env( ) = default;
         static tSptr create( char** envp, const tMap& default_parameters = { } );
   };

} // namespace carpc::tools::parameters
