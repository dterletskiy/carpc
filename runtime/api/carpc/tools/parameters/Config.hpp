#pragma once

#include "carpc/tools/parameters/Base.hpp"



namespace carpc::tools::parameters {

   class Config: public Base
   {
      public:
         // Argument name must be split with it's value by this delimiter
         static const tValue default_prefix;
         // Each argument name must start with this prefix
         static const tValue default_delimiter;

      private:
         Config( const tMap& map );
      public:
         ~Config( ) = default;
         static tSptr create( const std::string& file, const tMap& default_parameters = { } );
   };

} // namespace carpc::tools::parameters
