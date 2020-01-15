#pragma once

#include "api/sys/component/Component.hpp"
#include "api/sys/events/SysEvent.hpp"



namespace base {



class RootComponent
   : public Component
   , public ISysEventConsumer
{
public:
   RootComponent( const ServicePtr, const std::string& );
   ~RootComponent( ) override;

public:
   virtual bool boot( const std::string& ) = 0;
protected:
   virtual void exit( ) const final;

private:
   void process_event( const SysEvent& );
};



} // namespace base
