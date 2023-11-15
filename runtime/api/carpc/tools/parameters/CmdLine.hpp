#pragma once

#include "carpc/tools/parameters/Base.hpp"



namespace carpc::tools::parameters {

   class CmdLine: public Base
   {
      public:
         // Argument name must be split with it's value by this delimiter
         static const tValue default_prefix;
         // Each argument name must start with this prefix
         static const tValue default_delimiter;

      private:
         CmdLine( const tMap& map );
      public:
         ~CmdLine( ) = default;
         static tSptr create( int argc, char** argv, const tMap& default_parameters = { } );
   };

} // namespace carpc::tools::parameters
