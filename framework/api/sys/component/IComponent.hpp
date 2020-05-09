#pragma once

#include <memory>
#include "api/sys/service/IServiceThread.hpp"



namespace base {



class IComponent
{
public:
   using tSptr = std::shared_ptr< IComponent >;
   using tSptrList = std::list< tSptr >;
   using tCreator = IComponent::tSptr (*)( IServiceThread& );
   using tCreatorVector = std::vector< tCreator >;

public:
   IComponent( )  = default;
   virtual ~IComponent( ) = default;

public:
   virtual const std::string& name( ) const = 0;
   virtual const ID& id( ) const = 0;
   virtual const bool is_root( ) const = 0;

public:
   virtual void boot( const std::string& message = "boot" ) = 0;
   virtual void shutdown( const std::string& message = "shutdown" ) = 0;
};



} // namespace base
