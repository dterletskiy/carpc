#pragma once

#include "api/sys/component/Component.hpp"



namespace base {



class SysEventConsumer;



class RootComponent
   : public Component
{
public:
   RootComponent( const ServicePtr, const std::string& );
   ~RootComponent( ) override;

public:
   virtual bool boot( const std::string& info = "boot" ) = 0;
protected:
   virtual void shutdown( const std::string& info = "shutdown" ) const final;

private:
   std::shared_ptr< SysEventConsumer > mp_consumer = nullptr;
};



} // namespace base
