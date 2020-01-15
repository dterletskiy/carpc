#pragma once

#include "comm/Types.hpp"
#include "Types.hpp"



namespace base {






class IService
   : public std::enable_shared_from_this< IService >
{
public:
   IService( ) { };
   virtual ~IService( ) { };
   IService( const IService& ) = delete;
   IService& operator=( const IService& ) = delete;

public:
   virtual const TID id( ) const = 0;
   virtual const std::string& name( ) const = 0;
   virtual const size_t wd_timeout( ) const = 0;

public:
   virtual bool start( ) = 0;
   virtual void stop( ) = 0;
   virtual bool started( ) const = 0;
   virtual bool wait( ) = 0;

public:
   virtual bool send_event( const EventPtr ) = 0;
   virtual bool insert_event( const EventPtr ) = 0;
   virtual const uint64_t processed_events( ) const = 0;
   virtual const std::optional< time_t > process_started( ) const = 0;

public:
   virtual void set_notification( const Event_ID&, IEventConsumer* ) = 0;
   virtual void clear_notification( const Event_ID&, IEventConsumer* ) = 0;
   virtual void clear_all_notifications( ) = 0;
   virtual bool is_subscribed( const Event_ID& ) = 0;
};



} // namespace base
