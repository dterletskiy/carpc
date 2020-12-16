#pragma once

#include "api/sys/oswrappers/linux/timer.hpp"
#include "api/sys/comm/async/event/Event.hpp"
#include "api/sys/common/ID.hpp"
// #include "api/sys/application/Process.hpp"



namespace base {

   class ITimerConsumer;



   /**************************************************************
    *
    * Timer object MUST be created in application service thread.
    *
    *************************************************************/
   class Timer
   {
      public:
         using ID = base::TID< Timer >;
         static const std::size_t CONTINIOUS = std::numeric_limits< std::size_t >::max( );

      public:
         Timer( ITimerConsumer*, const std::string& name = "NoName_Timer" );
         ~Timer( );

         const bool operator<( const Timer& ) const;

         bool start( const std::size_t nanoseconds, const std::size_t count = CONTINIOUS );
         bool stop( );
         void process( const base::os::linux::timer::tID );

      public:
         const std::string& name( ) const;
      private:
         std::string             m_name = "NoName_Timer";

      public:
         const ID id( ) const;
      private:
         ID                      m_id = ID::invalid( );

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
         application::Context    m_context;
   };



   inline
   bool Timer::is_running( ) const
   {
      return m_is_running;
   }

   inline
   const Timer::ID Timer::id( ) const
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
      Timer::ID id;
   };
   DEFINE_EVENT( TimerEvent, TimerEventData, base::async::id::TSignature< Timer::ID::VALUE_TYPE > );



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

   using tCallback = std::function< void( const base::Timer::ID ) >;
   const std::size_t Infinite = std::numeric_limits< size_t >::max( );

   // This timer creates new thread and sleep it to some milliseconds.
   // When thread is finished "callback" will be executed in context of application thread
   // where this timer have been called.
   // Performance of this timer is worse then base::Timer implementation.
   // This timer can't be stopped.
   base::Timer::ID start( const size_t milliseconds, const size_t count, tCallback callback, const bool asynchronous = true );

} // namespace base::timer
