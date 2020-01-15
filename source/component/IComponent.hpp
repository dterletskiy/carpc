#pragma once

#include "service/Types.hpp"
#include "Types.hpp"



namespace base {



class IComponent
{
public:
   virtual ~IComponent( ) { }

public:
   virtual IServicePtr service( ) const = 0;
};



}
