#pragma once

#include "api/sys/oswrappers/ConditionVariable.hpp"
#include "api/sys/comm/event/IAsync.hpp"
#include "api/sys/component/IComponent.hpp"
#include "api/sys/service/IServiceThread.hpp"



namespace base {



class ServiceThread
   : public IServiceThread
{
public:
   using tSptr = std::shared_ptr< ServiceThread >;
   using tWptr = std::weak_ptr< ServiceThread >;
   using tSptrList = std::list< tSptr >;
private:
   struct Comparator { bool operator( )( const IAsync::ISignature* p_es1, const IAsync::ISignature* p_es2 ) const { return *p_es1 < *p_es2; } };
   using tEventConsumersMap = std::map< const IAsync::ISignature*, std::set< IAsync::IConsumer* >, Comparator >;

public:
   struct Info
   {
      using tVector = std::vector< Info >;
      std::string                m_name;
      IComponent::tCreatorVector m_component_creators;
      size_t                     m_wd_timeout;
   };

public:
   ServiceThread( const Info& );
   ~ServiceThread( );
   ServiceThread( const ServiceThread& ) = delete;
   ServiceThread& operator=( const ServiceThread& ) = delete;

private:
   const TID id( ) const override;
   const std::string& name( ) const override;
   std::string                   m_name;

private:
   const size_t wd_timeout( ) const override;
   const std::optional< time_t > process_started( ) const override;
   size_t                        m_wd_timeout = 0;
   std::optional< time_t >       m_process_started = std::nullopt;

private:
   bool start( ) override;
   void stop( ) override;
   bool started( ) const override;
   bool wait( ) override;
   void boot( const std::string& ) override;
   void shutdown( const std::string& ) override;
   void thread_loop( );
   os::ThreadPtr                 mp_thread;
   bool                          m_started = false;

private:
   bool insert_event( const IAsync::tSptr ) override;
   IAsync::tSptr get_event( );
   const uint64_t processed_events( ) const;
   std::deque< IAsync::tSptr >   m_events;
   os::ConditionVariable         m_buffer_cond_var;
   uint64_t                      m_processed_events = 0;

private:
   void set_notification( const IAsync::ISignature&, IAsync::IConsumer* ) override;
   void clear_notification( const IAsync::ISignature&, IAsync::IConsumer* ) override;
   void clear_all_notifications( const IAsync::ISignature&, IAsync::IConsumer* ) override;
   bool is_subscribed( const IAsync::tSptr );
   void notify( const IAsync::tSptr );
   tEventConsumersMap            m_event_consumers_map;

private:
   IComponent::tSptrList         m_components;
   IComponent::tCreatorVector    m_component_creators;
};



inline
const std::string& ServiceThread::name( ) const
{
   return m_name;
}

inline
const TID ServiceThread::id( ) const
{
   return mp_thread ? mp_thread->id( ) : 0;
}

inline
const size_t ServiceThread::wd_timeout( ) const
{
   return m_wd_timeout;
}

inline
const uint64_t ServiceThread::processed_events( ) const
{
   return m_processed_events;
}

inline
const std::optional< time_t > ServiceThread::process_started( ) const
{
   return m_process_started;
}

inline
bool ServiceThread::started( ) const
{
   return m_started;
}

inline
bool ServiceThread::wait( )
{
   m_started = mp_thread->join( );
   return !m_started;
}



} // namespace base
