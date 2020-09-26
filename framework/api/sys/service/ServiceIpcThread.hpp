#pragma once

#include "api/sys/oswrappers/Socket.hpp"
#include "api/sys/oswrappers/ConditionVariable.hpp"
#include "api/sys/comm/event/IAsync.hpp"
#include "api/sys/component/IComponent.hpp"
#include "api/sys/service/IServiceThread.hpp"



namespace base {

   class ServiceEventConsumer;
   class InterfaceEventConsumer;

   class ServiceIpcThread
      : public IServiceThread
   {
      private:
         std::shared_ptr< ServiceEventConsumer > mp_service_event_consumer = nullptr;
         std::shared_ptr< InterfaceEventConsumer > mp_interface_event_consumer = nullptr;

      public:
         using tSptr = std::shared_ptr< ServiceIpcThread >;
         using tWptr = std::weak_ptr< ServiceIpcThread >;
         using tSptrList = std::list< tSptr >;
      private:
         struct Comparator
         {
            bool operator( )( const base::async::IAsync::ISignature* p_es1, const base::async::IAsync::ISignature* p_es2 ) const
            { return *p_es1 < *p_es2; }
         };
         using tEventConsumersMap = std::map< const base::async::IAsync::ISignature*, std::set< base::async::IAsync::IConsumer* >, Comparator >;

      public:
         ServiceIpcThread( );
         ServiceIpcThread( const ServiceIpcThread& ) = delete;
         ~ServiceIpcThread( );
         ServiceIpcThread& operator=( const ServiceIpcThread& ) = delete;

      private:
         void boot( const std::string& ) override;
         void shutdown( const std::string& ) override;

      private:
         bool start( ) override;
         void stop( ) override;
         bool started( ) const override;
         bool wait( ) override;

      private:
         bool start_send( );
         void stop_send( );
         bool started_send( ) const;
         bool wait_send( );
         void thread_loop_send( );
         const base::os::Thread& thread( ) const override;
         os::Thread                                   m_thread_send;
         bool                                         m_started_send = false;

      private:
         bool start_receive( );
         void stop_receive( );
         bool started_receive( ) const;
         bool wait_receive( );
         void thread_loop_receive( );
         os::Thread                                   m_thread_receive;
         bool                                         m_started_receive = false;

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
         bool setup_connection( );
         os::Socket                                   m_socket_sb;
         os::Socket                                   m_socket_master;
         os::Socket::tSptrList                        m_sockets_slave;
         os::Socket::tSptrList                        m_sockets_client;

      public:
         bool send( os::Socket&, const base::async::IAsync::tSptr );
         bool send( const base::async::IAsync::tSptr );
         bool send( os::Socket& _socket, dsi::tByteStream& );
         bool send( dsi::tByteStream& );

      private:
         bool process_packet( dsi::Packet&, os::Socket& );
         bool process_package( dsi::Package&, os::Socket& );
   };



   inline
   const base::os::Thread& ServiceIpcThread::thread( ) const
   {
      return m_thread_send;
   }

   inline
   const uint64_t ServiceIpcThread::processed_events( ) const
   {
      return m_processed_events;
   }

   inline
   bool ServiceIpcThread::started( ) const
   {
      return started_send( ) && started_receive( );
   }

   inline
   bool ServiceIpcThread::wait( )
   {
      return wait_send( ) && wait_receive( );
   }

   inline
   bool ServiceIpcThread::started_send( ) const
   {
      return m_started_send;
   }

   inline
   bool ServiceIpcThread::wait_send( )
   {
      m_started_send = m_thread_send.join( );
      return !m_started_send;
   }

   inline
   bool ServiceIpcThread::started_receive( ) const
   {
      return m_started_receive;
   }

   inline
   bool ServiceIpcThread::wait_receive( )
   {
      m_started_receive = m_thread_receive.join( );
      return !m_started_receive;
   }



} // namespace base
