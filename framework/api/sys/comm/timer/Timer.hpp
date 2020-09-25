#pragma once

#include "api/sys/oswrappers/linux/timer.hpp"
#include "api/sys/comm/event/Event.hpp"



namespace base {



   class ITimerConsumer;

   class Timer
   {
      public:
         using ID = os::linux::timer::TimerID;

      public:
         Timer( ITimerConsumer* );
         ~Timer( );

         const bool operator<( const Timer& ) const;

         bool start( const long int nanoseconds );
         bool stop( );

      public:
         bool is_running( ) const;
      private:
         bool              m_is_running = false;

      public:
         const ID id( ) const;
      private:
         ID                m_id = nullptr;

      public:
         long int nanoseconds( ) const;
      private:
         long int          m_nanoseconds = 0;

      private:
         ITimerConsumer*   mp_consumer = nullptr;
   };



   struct TimerEventData
   {
      Timer::ID id;
   };
   DEFINE_EVENT( TimerEvent, TimerEventData, TSignatureID< size_t > );



   class ITimerConsumer
      : public TimerEvent::Consumer
   {
      public:
         ITimerConsumer( );
         virtual ~ITimerConsumer( );

         virtual void process_timer( const Timer::ID ) = 0;

      private:
         void process_event( const TimerEvent::Event& ) override;
   };



} // namespace base



namespace base::timer {

   const std::size_t Infinite = std::numeric_limits< size_t >::max( );

   // This timer creates new thread and sleep it to some milliseconds. When thread is finished "callback" will be executed in context of service
   // where this timer have been called.
   // Performance of this timer is worse then base::Timer implementation.
   // This timer can't be stopped.
   ID start( const size_t milliseconds, const size_t count, std::function< void( const ID ) > callback, const bool asynchronous = true );

} // namespace base::timer
