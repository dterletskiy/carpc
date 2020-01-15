#pragma once

#include "comm/Types.hpp"
#include "Types.hpp"



namespace base {



class IServiceBrocker
   : public std::enable_shared_from_this< IServiceBrocker >
{
public:
   virtual ~IServiceBrocker( ) { };

public:
   virtual bool start( ) = 0;
   virtual void stop( ) = 0;
   virtual bool started( ) const = 0;
   virtual bool wait( ) = 0;

public:
   virtual bool insert_event( const EventPtr ) = 0;
};



} // namespace base
