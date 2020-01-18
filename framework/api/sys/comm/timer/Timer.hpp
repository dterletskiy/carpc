#pragma once

#include "api/sys/oswrappers/linux/timer.hpp"
#include "api/sys/comm/event/Event.hpp"



namespace base {



using TimerID = os::linux::timer::TimerID;
class ITimerConsumer;

class Timer
{
public:
   Timer( ITimerConsumer* );
   ~Timer( );

   bool start( const long int microseconds );
   bool stop( );

public:
   bool is_running( ) const;
private:
   bool              m_is_running = false;

public:
   const TimerID id( ) const;
private:
   TimerID           m_id = nullptr;

public:
   long int microseconds( ) const;
private:
   long int          m_microseconds = 0;

private:
   ITimerConsumer*   mp_consumer = nullptr;
};



struct TimerEventData
{
   TimerID id;
};
DECLARE_EVENT( TimerEvent, TimerEventData, size_t );



class ITimerConsumer
   : public TimerEvent::Consumer
{
public:
   ITimerConsumer( );
   virtual ~ITimerConsumer( );

   virtual void process_timer( const TimerID ) = 0;

private:
   void process_event( const TimerEvent::Event& ) override;
};



} // namespace base
