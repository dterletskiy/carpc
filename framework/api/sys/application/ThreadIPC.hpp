#pragma once

#include <atomic>

#include "api/sys/application/IThread.hpp"



namespace carpc::application {

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
         const carpc::os::Thread& thread( ) const override;
         void thread_loop( );
         carpc::os::Thread                            m_thread;
         std::atomic< bool >                          m_started = false;

      private:
         bool insert_event( const carpc::async::IAsync::tSptr ) override;
         carpc::async::IAsync::tSptr get_event( );
         tEventCollection                             m_event_queue;

      private:
         void set_notification( const carpc::async::IAsync::ISignature::tSptr, carpc::async::IAsync::IConsumer* ) override;
         void clear_notification( const carpc::async::IAsync::ISignature::tSptr, carpc::async::IAsync::IConsumer* ) override;
         void clear_all_notifications( const carpc::async::IAsync::ISignature::tSptr, carpc::async::IAsync::IConsumer* ) override;
         bool is_subscribed( const carpc::async::IAsync::tSptr );
         void notify( const carpc::async::IAsync::tSptr );
         tConsumerMap                                 m_consumers_map;

      private:
         void dump( ) const override;

      public:
         bool send( const carpc::async::IAsync::tSptr, const application::Context& ) override;
      private:
         SendReceive*                                 mp_send_receive;
   };



   inline
   const carpc::os::Thread& ThreadIPC::thread( ) const
   {
      return m_thread;
   }

} // namespace carpc::application
