#pragma once

#include "Types.hpp"



namespace base {



class IServiceProcess
{
public:
   virtual ~IServiceProcess( ) { };

public:
   virtual bool init( const ServiceInfoVector& ) = 0;
   virtual void boot( ) = 0;

public:
   virtual IServiceBrockerPtr service_brocker( ) const = 0;

public:
   virtual IServicePtr service( const TID& ) const = 0;
   virtual IServicePtr current_service( ) const = 0;
   virtual IServicePtrList service_list( ) const = 0;
};




} // namespace base
