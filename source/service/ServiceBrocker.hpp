#pragma once

#include "oswrappers/Thread.hpp"
#include "oswrappers/ConditionVariable.hpp"
#include "comm/Event.hpp"
#include "IServiceBrocker.hpp"



namespace base {



class ServiceBrocker
   : public IServiceBrocker
{
public:
   ~ServiceBrocker( ) override;
   static IServiceBrockerPtr instance( );
private:
   ServiceBrocker( );
   ServiceBrocker( const ServiceBrocker& ) = delete;
   ServiceBrocker& operator=( const ServiceBrocker& ) = delete;
   static IServiceBrockerPtr mp_instance;

public:
   bool start( ) override;
   void stop( ) override;
   bool started( ) const override;
   bool wait( ) override;
private:
   void thread_loop( );
   os::ThreadPtr                 mp_thread;
   bool                          m_started = false;

public:
   bool insert_event( const EventPtr ) override;
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
