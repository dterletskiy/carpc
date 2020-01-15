#pragma once

#include "common/Types.hpp"



namespace base {

   class IComponent;
   using IComponentPtr        = std::shared_ptr< IComponent >;
   using IComponentPtrList    = std::list< IComponentPtr >;

} // namespace base
