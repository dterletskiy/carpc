#pragma once

#include "api/sys/oswrappers/ConditionVariable.hpp"
#include "api/sys/comm/event/IAsync.hpp"
#include "api/sys/component/IComponent.hpp"
#include "api/sys/service/IServiceThread.hpp"



namespace base {



class ServiceIpcThread
   : public IServiceThread
{
public:
   using tSptr = std::shared_ptr< ServiceIpcThread >;
   using tWptr = std::weak_ptr< ServiceIpcThread >;
   using tSptrList = std::list< tSptr >;
private:
   struct Comparator { bool operator( )( const IAsync::ISignature* p_es1, const IAsync::ISignature* p_es2 ) const { return *p_es1 < *p_es2; } };
   using tEventConsumersMap = std::map< const IAsync::ISignature*, std::set< IAsync::IConsumer* >, Comparator >;

public:
   ~ServiceIpcThread( );
   static tSptr instance( );
private:
   ServiceIpcThread( );
   ServiceIpcThread( const ServiceIpcThread& ) = delete;
   ServiceIpcThread& operator=( const ServiceIpcThread& ) = delete;
   static tSptr                  mp_instance;

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
   void boot( const std::string& ) override;
   void shutdown( const std::string& ) override;

private:
   bool start( ) override;
   void stop( ) override;
   bool started( ) const override;
   bool wait( ) override;

private:
   bool start_send( );
   void stop_send( );
   bool started_send( ) const;
   bool wait_send( );
   void thread_loop_send( );
   os::ThreadPtr                 mp_thread_send;
   bool                          m_started_send = false;

private:
   bool start_receive( );
   void stop_receive( );
   bool started_receive( ) const;
   bool wait_receive( );
   void thread_loop_receive( );
   os::ThreadPtr                 mp_thread_receive;
   bool                          m_started_receive = false;

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
   bool setup_connection( );
   int                           m_master_socket = -1;
};



inline
const std::string& ServiceIpcThread::name( ) const
{
   return m_name;
}

inline
const TID ServiceIpcThread::id( ) const
{
   return mp_thread_send ? mp_thread_send->id( ) : 0;
}

inline
const size_t ServiceIpcThread::wd_timeout( ) const
{
   return m_wd_timeout;
}

inline
const uint64_t ServiceIpcThread::processed_events( ) const
{
   return m_processed_events;
}

inline
const std::optional< time_t > ServiceIpcThread::process_started( ) const
{
   return m_process_started;
}

inline
bool ServiceIpcThread::started( ) const
{
   return started_send( ) && started_receive( );
}

inline
bool ServiceIpcThread::wait( )
{
   return wait_send( ) && wait_receive( );
}

inline
bool ServiceIpcThread::started_send( ) const
{
   return m_started_send;
}

inline
bool ServiceIpcThread::wait_send( )
{
   m_started_send = mp_thread_send->join( );
   return !m_started_send;
}

inline
bool ServiceIpcThread::started_receive( ) const
{
   return m_started_receive;
}

inline
bool ServiceIpcThread::wait_receive( )
{
   m_started_receive = mp_thread_receive->join( );
   return !m_started_receive;
}



} // namespace base
