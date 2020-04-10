#pragma once

#include <memory>
#include "api/sys/service/IServiceThread.hpp"



namespace base {



class IComponent
{
public:
   using tSptr = std::shared_ptr< IComponent >;
   using tSptrList = std::list< tSptr >;
   using tCreator = IComponent::tSptr (*)( IServiceThread::tSptr );
   using tCreatorVector = std::vector< tCreator >;

public:
   IComponent( )  = default;
   virtual ~IComponent( ) = default;

public:
   virtual const std::string& name( ) const = 0;
   virtual const ID& id( ) const = 0;
};



} // namespace base
