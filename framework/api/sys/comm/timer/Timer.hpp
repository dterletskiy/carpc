#pragma once

#include "api/sys/oswrappers/linux/timer.hpp"
#include "api/sys/comm/event/Event.hpp"
#include "api/sys/service/IServiceThread.hpp"



namespace base {

   class ITimerConsumer;



   class Timer
   {
      public:
         using tID = base::ID;
         static const std::size_t CONTINIOUS = std::numeric_limits< std::size_t >::max( );

      public:
         Timer( ITimerConsumer* );
         ~Timer( );

         const bool operator<( const Timer& ) const;

         bool start( const std::size_t nanoseconds, const std::size_t count = CONTINIOUS );
         bool stop( );
         void process( const base::os::linux::timer::tID );

      public:
         const base::ID id( ) const;
      private:
         base::ID                m_id = InvalidID;

      public:
         bool is_running( ) const;
      private:
         bool                    m_is_running = false;

      public:
         const base::os::linux::timer::tID timer_id( ) const;
      private:
         base::os::linux::timer::tID   m_timer_id = nullptr;

      public:
         std::size_t nanoseconds( ) const;
         std::size_t count( ) const;
         std::size_t ticks( ) const;
      private:
         std::size_t             m_nanoseconds = 0;
         std::size_t             m_count = 0;
         std::size_t             m_ticks = 0;

      private:
         ITimerConsumer*         mp_consumer = nullptr;
         IServiceThread::tWptr   mp_service;
   };



   inline
   bool Timer::is_running( ) const
   {
      return m_is_running;
   }

   inline
   const base::ID Timer::id( ) const
   {
      return m_id;
   }

   inline
   const base::os::linux::timer::tID Timer::timer_id( ) const
   {
      return m_timer_id;
   }

   inline
   std::size_t Timer::nanoseconds( ) const
   {
      return m_nanoseconds;
   }

   inline
   std::size_t Timer::count( ) const
   {
      return m_count;
   }

   inline
   std::size_t Timer::ticks( ) const
   {
      return m_ticks;
   }



   struct TimerEventData
   {
      base::ID id;
   };
   DEFINE_EVENT( TimerEvent, TimerEventData, base::async::TSignatureID< size_t > );



   class ITimerConsumer
      : public TimerEvent::Consumer
   {
      public:
         ITimerConsumer( );
         virtual ~ITimerConsumer( );

         virtual void process_timer( const base::ID ) = 0;

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
