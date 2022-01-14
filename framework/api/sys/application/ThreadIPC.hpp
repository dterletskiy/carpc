#pragma once

#include <atomic>

#include "api/sys/application/IThread.hpp"



namespace base::application {

   class Context;
   class SendReceive;



   class ThreadIPC : public IThread
   {
      public:
         ThreadIPC( );
         ~ThreadIPC( );
         ThreadIPC( const ThreadIPC& ) = delete;
         ThreadIPC& operator=( const ThreadIPC& ) = delete;

      private:
         void boot( const std::string& ) override;
         void shutdown( const std::string& ) override;

      private:
         bool start( ) override;
         void stop( ) override;
         bool started( ) const override;
         bool wait( ) override;

      private:
         const base::os::Thread& thread( ) const override;
         void thread_loop( );
         base::os::Thread                             m_thread;
         std::atomic< bool >                          m_started = false;

      private:
         bool insert_event( const base::async::IAsync::tSptr ) override;
         base::async::IAsync::tSptr get_event( );
         tEventCollection                             m_event_queue;

      private:
         void set_notification( const base::async::IAsync::ISignature::tSptr, base::async::IAsync::IConsumer* ) override;
         void clear_notification( const base::async::IAsync::ISignature::tSptr, base::async::IAsync::IConsumer* ) override;
         void clear_all_notifications( const base::async::IAsync::ISignature::tSptr, base::async::IAsync::IConsumer* ) override;
         bool is_subscribed( const base::async::IAsync::tSptr );
         void notify( const base::async::IAsync::tSptr );
         tConsumerMap                                 m_consumers_map;

      private:
         void dump( ) const override;

      public:
         bool send( const base::async::IAsync::tSptr, const application::Context& ) override;
      private:
         SendReceive*                                 mp_send_receive;
   };



   inline
   const base::os::Thread& ThreadIPC::thread( ) const
   {
      return m_thread;
   }

   inline
   bool ThreadIPC::started( ) const
   {
      return m_started.load( );
   }

} // namespace base::application
