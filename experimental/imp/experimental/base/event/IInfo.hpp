#pragma once

#include <memory>
#include "imp/experimental/base/event/Types.hpp"



namespace fw::event {

   struct IInfo
   {
      IInfo( ) = default;
      IInfo( const IInfo& ) = default;
      ~IInfo( ) = default;

      virtual bool operator<( const IInfo& ) const = 0;

      virtual const std::string to_string( ) const = 0;

      virtual const std::shared_ptr< IInfo > duplicate( ) const = 0;
   };

} // namespace fw::event
