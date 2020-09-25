#pragma once

#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/comm/event/IAsync.hpp"



namespace base {



class IEvent;



class IServiceThread
   : public std::enable_shared_from_this< IServiceThread >
{
   friend class IEvent;

public:
   using tSptr = std::shared_ptr< IServiceThread >;
   using tWptr = std::weak_ptr< IServiceThread >;
   using tSptrList = std::list< tSptr >;

public:
   IServiceThread( ) = default;
   virtual ~IServiceThread( ) = default;
   IServiceThread( const IServiceThread& ) = delete;
   IServiceThread& operator=( const IServiceThread& ) = delete;

public:
   virtual const TID id( ) const = 0;
   virtual const std::string& name( ) const = 0;

public:
   virtual bool start( ) = 0;
   virtual void stop( ) = 0;
   virtual bool started( ) const = 0;
   virtual bool wait( ) = 0;
   virtual void boot( const std::string& ) = 0;
   virtual void shutdown( const std::string& ) = 0;

public:
   virtual void set_notification( const base::async::IAsync::ISignature&, base::async::IAsync::IConsumer* ) = 0;
   virtual void clear_notification( const base::async::IAsync::ISignature&, base::async::IAsync::IConsumer* ) = 0;
   virtual void clear_all_notifications( const base::async::IAsync::ISignature&, base::async::IAsync::IConsumer* ) = 0;
   virtual bool insert_event( const base::async::IAsync::tSptr ) = 0;

public:
   virtual const size_t wd_timeout( ) const = 0;
   virtual const std::optional< time_t > process_started( ) const = 0;
};



} // namespace base
