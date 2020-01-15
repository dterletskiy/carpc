#pragma once

#include "api/sys/component/Component.hpp"



namespace base {



class RootComponent
   : public Component
{
public:
   RootComponent( const ServicePtr, const std::string& );
   ~RootComponent( ) override;

public:
   virtual bool boot( const std::string& ) = 0;
protected:
   virtual void exit( ) const final;
};



} // namespace base
