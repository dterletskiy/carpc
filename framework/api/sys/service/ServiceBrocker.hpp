#pragma once

#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/oswrappers/ConditionVariable.hpp"
#include "api/sys/comm/Types.hpp"
#include "api/sys/service/Types.hpp"



namespace base {



class ServiceBrocker
   : public std::enable_shared_from_this< ServiceBrocker >
{
public:
   ~ServiceBrocker( );
   static ServiceBrockerPtr instance( );
private:
   ServiceBrocker( );
   ServiceBrocker( const ServiceBrocker& ) = delete;
   ServiceBrocker& operator=( const ServiceBrocker& ) = delete;
   static ServiceBrockerPtr mp_instance;

public:
   bool start( );
   void stop( );
   bool started( ) const;
   bool wait( );
private:
   void thread_loop( );
   os::ThreadPtr                 mp_thread;
   bool                          m_started = false;

public:
   bool insert_event( const EventPtr );
private:
   EventPtr get_event( );
   void notify( const EventPtr );
   std::deque< EventPtr >        m_events;
   os::ConditionVariable         m_buffer_cond_var;
};



inline
bool ServiceBrocker::started( ) const
{
   return m_started;
}

inline
bool ServiceBrocker::wait( )
{
   m_started = mp_thread->join( );
   return !m_started;
}



} // namespace base
