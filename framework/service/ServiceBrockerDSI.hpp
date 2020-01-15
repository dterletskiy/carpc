#pragma once

#include "oswrappers/Thread.hpp"
#include "oswrappers/ConditionVariable.hpp"
#include "comm/Event.hpp"
#include "IServiceBrockerDSI.hpp"



namespace base {



class ServiceBrockerDSI
   : public IServiceBrockerDSI
{
public:
   ~ServiceBrockerDSI( ) override;
   static IServiceBrockerDsiPtr instance( );
private:
   ServiceBrockerDSI( );
   ServiceBrockerDSI( const ServiceBrockerDSI& ) = delete;
   ServiceBrockerDSI& operator=( const ServiceBrockerDSI& ) = delete;
   static IServiceBrockerDsiPtr mp_instance;

public:
   bool start( ) override;
   void stop( ) override;
   bool started( ) const override;
   bool wait( ) override;

public:
   bool start_send( );
   void stop_send( );
   bool started_send( ) const;
   bool wait_send( );
private:
   void thread_loop_send( );
   os::ThreadPtr                 mp_thread_send;
   bool                          m_started_send = false;

public:
   bool start_receive( );
   void stop_receive( );
   bool started_receive( ) const;
   bool wait_receive( );
private:
   void thread_loop_receive( );
   os::ThreadPtr                 mp_thread_receive;
   bool                          m_started_receive = false;

public:
   bool insert_event( const EventPtr ) override;
private:
   EventPtr get_event( );
   void notify( const EventPtr );
   std::deque< EventPtr >        m_events;
   os::ConditionVariable         m_buffer_cond_var;

private:
   bool setup_connection( );
   int                           m_master_socket;
};



inline
bool ServiceBrockerDSI::started( ) const
{
   return started_send( ) && started_receive( );
}

inline
bool ServiceBrockerDSI::wait( )
{
   return wait_send( ) && wait_receive( );
}

inline
bool ServiceBrockerDSI::started_send( ) const
{
   return m_started_send;
}

inline
bool ServiceBrockerDSI::wait_send( )
{
   m_started_send = mp_thread_send->join( );
   return !m_started_send;
}

inline
bool ServiceBrockerDSI::started_receive( ) const
{
   return m_started_receive;
}

inline
bool ServiceBrockerDSI::wait_receive( )
{
   m_started_receive = mp_thread_receive->join( );
   return !m_started_receive;
}



} // namespace base
