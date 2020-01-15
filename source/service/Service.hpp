#pragma once

#include "oswrappers/Thread.hpp"
#include "oswrappers/ConditionVariable.hpp"
#include "component/Types.hpp"
#include "events/Events.hpp"
#include "IService.hpp"



namespace base {



class Service
   : public IService
   , public IServiceEventConsumer
   , public DsiService::IDsiServiceEventConsumer
{
public:
   Service( IServiceBrockerPtr, const ServiceInfo& );
   ~Service( ) override;
   Service( const Service& ) = delete;
   Service& operator=( const Service& ) = delete;

public:
   const TID id( ) const override;

public:
   const std::string& name( ) const override;
private:
   std::string             m_name;

public:
   const size_t wd_timeout( ) const override;
private:
   size_t                  m_wd_timeout;

private:
   IServiceBrockerPtrW     mp_service_brocker;

public:
   bool start( ) override;
   void stop( ) override;
   bool started( ) const override;
   bool wait( ) override;
private:
   void thread_loop( );
   os::ThreadPtr           mp_thread;
   bool                    m_started = false;

public:
   bool send_event( const EventPtr ) override;
   bool insert_event( const EventPtr ) override;
private:
   EventPtr get_event( );
   const uint64_t processed_events( ) const override;
   std::deque< EventPtr >  m_events;
   os::ConditionVariable   m_buffer_cond_var;
   uint64_t                m_processed_events = 0;

public:
   void set_notification( const Event_ID&, IEventConsumer* ) override;
   void clear_notification( const Event_ID&, IEventConsumer* ) override;
   void clear_all_notifications( ) override;
   bool is_subscribed( const Event_ID& ) override;
private:
   void notify( const EventPtr );
   std::map< Event_ID, std::set< IEventConsumer* > > m_event_consumers_map;
   const std::optional< time_t > process_started( ) const override;
   std::optional< time_t > m_process_started;

protected:
   void process_event( const ServiceEvent& ) override;
   void process_event( const DsiService::DsiServiceEvent& ) override;

private:
   IComponentPtrList       m_components;
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
