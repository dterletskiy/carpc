#pragma once

#include "events/Events.hpp"



namespace base {



class IRoot
   : public ISysEventConsumer
{
public:
   IRoot( ) { }
   virtual ~IRoot( ) { }

   virtual bool boot( const std::string& ) = 0;
   virtual void process_event( const SysEvent& ) = 0;
};



} // namespace base
