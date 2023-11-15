#pragma once

#include <cstdlib>
#include <cstdint>
#include <string>
#include <map>
#include <optional>
#include <sstream>



namespace carpc::tools::parameters {

   struct Description
   {
      std::string name;
      std::string type;
      std::string value;
      bool required;
   };

   using tArgument = std::string;
   using tParameter = std::string;
   using tValue = std::string;
   using tValueOpt = std::optional< tValue >;
   using tMap = std::map< tParameter, tValueOpt >;
   using tResult = std::pair< tValueOpt, bool >;

   static const tValueOpt invalid_value{ std::nullopt };

   namespace cmdline {

      static const tValue default_prefix{ "--" };
      static const tValue default_delimiter{ "=" };

   }

   namespace env {

      static const tValue default_prefix{ "" };
      static const tValue default_delimiter{ "=" };

   }

   namespace config {

      static const tValue default_prefix{ "--" };
      static const tValue default_delimiter{ "=" };

   }

} // namespace carpc::tools::parameters
