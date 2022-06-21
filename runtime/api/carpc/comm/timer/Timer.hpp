#pragma once

#include <cstdlib>
#include <limits>
#include <functional>

#include "carpc/oswrappers/linux/timer.hpp"
#include "carpc/comm/async/event/Event.hpp"
#include "carpc/comm/timer/Types.hpp"



namespace carpc::timer {

   class ITimerConsumer;



   /**************************************************************
    *
    * Timer object MUST be created in application service thread.
    *
    *************************************************************/
   class Timer
   {
      public:
         static const std::size_t CONTINIOUS = std::numeric_limits< std::size_t >::max( );

      public:
         Timer( ITimerConsumer*, const std::string& name = "NoName_Timer" );
         ~Timer( );

         const bool operator<( const Timer& ) const;

         bool start( const std::size_t nanoseconds, const std::size_t count = CONTINIOUS );
         bool stop( );
         void process( const carpc::os::os_linux::timer::tID );

      public:
         const std::string& name( ) const;
      private:
         comm::timer::Name       m_name{ "NoName_Timer" };

      public:
         const comm::timer::ID id( ) const;
      private:
         comm::timer::ID         m_id = comm::timer::ID::generate( );

      public:
         bool is_running( ) const;
      private:
         bool                    m_is_running = false;

      public:
         const carpc::os::os_linux::timer::tID timer_id( ) const;
      private:
         carpc::os::os_linux::timer::tID   m_timer_id = nullptr;

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
         application::Context    m_context = application::Context::current( );
   };



   inline
   bool Timer::is_running( ) const
   {
      return m_is_running;
   }

   inline
   const comm::timer::ID Timer::id( ) const
   {
      return m_id;
   }

   inline
   const carpc::os::os_linux::timer::tID Timer::timer_id( ) const
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
      comm::timer::ID id;
   };
   DEFINE_EVENT( TimerEvent, TimerEventData, carpc::async::id::TSignature< comm::timer::ID::VALUE_TYPE > );



   class ITimerConsumer
      : public TimerEvent::Consumer
   {
      public:
         ITimerConsumer( );
         virtual ~ITimerConsumer( );

         virtual void process_timer( const comm::timer::ID ) = 0;

      private:
         void process_event( const TimerEvent::Event& ) override;
   };



} // namespace carpc



namespace carpc::timer {

   using tCallback = std::function< void( const carpc::comm::timer::ID ) >;
   const std::size_t Infinite = std::numeric_limits< std::size_t >::max( );

   // This timer creates new thread and sleep it to some milliseconds.
   // When thread is finished "callback" will be executed in context of application thread
   // where this timer have been called.
   // Performance of this timer is worse then carpc::timer::Timer implementation.
   // This timer can't be stopped.
   carpc::comm::timer::ID start( const std::size_t milliseconds, const std::size_t count, tCallback callback, const bool asynchronous = true );

} // namespace carpc::timer
