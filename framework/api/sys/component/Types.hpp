#pragma once

#include "api/sys/common/Types.hpp"



namespace base {

   class Component;
   using ComponentPtr         = std::shared_ptr< Component >;
   using ComponentPtrList     = std::list< ComponentPtr >;

} // namespace base
