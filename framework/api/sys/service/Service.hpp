#pragma once

#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/oswrappers/ConditionVariable.hpp"
#include "api/sys/component/Types.hpp"
#include "api/sys/comm/Types.hpp"
#include "api/sys/comm/Event.hpp"
#include "api/sys/service/Types.hpp"



namespace base {



class Service
   : public std::enable_shared_from_this< Service >
{
public:
   Service( ServiceBrockerPtr, const ServiceInfo& );
   ~Service( );
   Service( const Service& ) = delete;
   Service& operator=( const Service& ) = delete;

public:
   const TID id( ) const;

public:
   const std::string& name( ) const;
private:
   std::string             m_name;

public:
   const size_t wd_timeout( ) const;
private:
   size_t                  m_wd_timeout;

private:
   ServiceBrockerPtrW     mp_service_brocker;

public:
   bool start( );
   void stop( );
   bool started( ) const;
   bool wait( );
private:
   void thread_loop( );
   os::ThreadPtr           mp_thread;
   bool                    m_started = false;

public:
   bool send_event( const EventPtr );
   bool insert_event( const EventPtr );
private:
   EventPtr get_event( );
   const uint64_t processed_events( ) const;
   std::deque< EventPtr >  m_events;
   os::ConditionVariable   m_buffer_cond_var;
   uint64_t                m_processed_events = 0;

public:
   void set_notification( const EventTypeID&, IEventConsumer* );
   void clear_notification( const EventTypeID&, IEventConsumer* );
   void clear_all_notifications( );
   bool is_subscribed( const EventTypeID& );
   bool is_subscribed( const EventPtr );
   const std::optional< time_t > process_started( ) const;
private:
   void notify( const EventPtr );
   std::map< EventTypeID, std::set< IEventConsumer* > > m_event_consumers_map;
   std::optional< time_t > m_process_started = std::nullopt;

private:
   ComponentPtrList        m_components;
   ComponentCreatorVector  m_component_creators;
};



inline
const std::string& Service::name( ) const
{
   return m_name;
}

inline
const size_t Service::wd_timeout( ) const
{
   return m_wd_timeout;
}

inline
const uint64_t Service::processed_events( ) const
{
   return m_processed_events;
}

inline
const std::optional< time_t > Service::process_started( ) const
{
   return m_process_started;
}

inline
bool Service::started( ) const
{
   return m_started;
}

inline
bool Service::wait( )
{
   m_started = mp_thread->join( );
   return !m_started;
}



} // namespace base
