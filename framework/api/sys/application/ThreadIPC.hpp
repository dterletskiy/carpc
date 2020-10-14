#pragma once

#include "api/sys/oswrappers/ConditionVariable.hpp"
#include "api/sys/comm/event/IAsync.hpp"
#include "api/sys/component/IComponent.hpp"
#include "api/sys/application/SendReceive.hpp"
#include "api/sys/application/IThread.hpp"



namespace base::application {

   class Context;



   class ThreadIPC : public IThread
   {
      public:
         using tSptr = std::shared_ptr< ThreadIPC >;
         using tWptr = std::weak_ptr< ThreadIPC >;
         using tSptrList = std::list< tSptr >;

      private:
         struct Comparator
         {
            bool operator( )( const base::async::IAsync::ISignature* p_es1, const base::async::IAsync::ISignature* p_es2 ) const
            { return *p_es1 < *p_es2; }
         };
         using tEventConsumersMap = std::map< const base::async::IAsync::ISignature*, std::set< base::async::IAsync::IConsumer* >, Comparator >;

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
         bool                                         m_started = false;

      private:
         bool insert_event( const base::async::IAsync::tSptr ) override;
         base::async::IAsync::tSptr get_event( );
         const uint64_t processed_events( ) const;
         std::deque< base::async::IAsync::tSptr >     m_events;
         os::ConditionVariable                        m_buffer_cond_var;
         uint64_t                                     m_processed_events = 0;

      private:
         void set_notification( const base::async::IAsync::ISignature&, base::async::IAsync::IConsumer* ) override;
         void clear_notification( const base::async::IAsync::ISignature&, base::async::IAsync::IConsumer* ) override;
         void clear_all_notifications( const base::async::IAsync::ISignature&, base::async::IAsync::IConsumer* ) override;
         bool is_subscribed( const base::async::IAsync::tSptr );
         void notify( const base::async::IAsync::tSptr );
         tEventConsumersMap                           m_event_consumers_map;

      private:
         void dump( ) const;



      public:
         bool insert_ipc_event( const base::async::IAsync::tSptr, const application::Context& to_context );
         base::async::IAsync::tSptr get_ipc_event( );
         const uint64_t processed_ipc_events( ) const;
      private:
         std::deque< base::async::IAsync::tSptr >     m_ipc_events;
         os::ConditionVariable                        m_ipc_buffer_cond_var;
         uint64_t                                     m_ipc_processed_events = 0;
         SendReceive                                  m_send_receive;
   };



   inline
   const base::os::Thread& ThreadIPC::thread( ) const
   {
      return m_thread;
   }

   inline
   bool ThreadIPC::started( ) const
   {
      return m_started;
   }

   inline
   bool ThreadIPC::wait( )
   {
      m_started = m_thread.join( );
      const bool stopped = m_send_receive.wait( );
      return !m_started && !stopped;
   }

   inline
   const uint64_t ThreadIPC::processed_events( ) const
   {
      return m_processed_events;
   }

   inline
   const uint64_t ThreadIPC::processed_ipc_events( ) const
   {
      return m_ipc_processed_events;
   }

} // namespace base::application
