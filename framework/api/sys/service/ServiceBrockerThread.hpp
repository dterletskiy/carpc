#pragma once

#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/oswrappers/ConditionVariable.hpp"
#include "api/sys/comm/event/IEvent.hpp"



namespace base {



class IEvent;



class ServiceBrockerThread
   : public std::enable_shared_from_this< ServiceBrockerThread >
{
   friend class IEvent;

public:
   using tSptr = std::shared_ptr< ServiceBrockerThread >;

public:
   ~ServiceBrockerThread( );
   static tSptr instance( );
private:
   ServiceBrockerThread( );
   ServiceBrockerThread( const ServiceBrockerThread& ) = delete;
   ServiceBrockerThread& operator=( const ServiceBrockerThread& ) = delete;
   static tSptr                  mp_instance;

public:
   bool start( );
   void stop( );
   bool started( ) const;
   bool wait( );

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
   bool insert_event( const IEvent::tSptr );
   IEvent::tSptr get_event( );
   std::deque< IEvent::tSptr >   m_events;
   os::ConditionVariable         m_buffer_cond_var;

private:
   bool setup_connection( );
   int                           m_master_socket;
};



inline
bool ServiceBrockerThread::started( ) const
{
   return started_send( ) && started_receive( );
}

inline
bool ServiceBrockerThread::wait( )
{
   return wait_send( ) && wait_receive( );
}

inline
bool ServiceBrockerThread::started_send( ) const
{
   return m_started_send;
}

inline
bool ServiceBrockerThread::wait_send( )
{
   m_started_send = mp_thread_send->join( );
   return !m_started_send;
}

inline
bool ServiceBrockerThread::started_receive( ) const
{
   return m_started_receive;
}

inline
bool ServiceBrockerThread::wait_receive( )
{
   m_started_receive = mp_thread_receive->join( );
   return !m_started_receive;
}



} // namespace base
