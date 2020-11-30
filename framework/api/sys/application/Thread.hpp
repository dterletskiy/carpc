#pragma once

#include "api/sys/component/IComponent.hpp"
#include "api/sys/application/IThread.hpp"



namespace base::application {

   class Thread : public IThread
   {
      public:
         struct Configuration
         {
            using tVector = std::vector< Configuration >;
            std::string                m_name;
            component::IComponent::tCreatorVector
                                       m_component_creators;
            size_t                     m_wd_timeout;
         };

      public:
         Thread( const Configuration& );
         ~Thread( );
         Thread( const Thread& ) = delete;
         Thread& operator=( const Thread& ) = delete;

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
         tEventCollection                             m_event_queue;

      private:
         void set_notification( const base::async::IAsync::ISignature&, base::async::IAsync::IConsumer* ) override;
         void clear_notification( const base::async::IAsync::ISignature&, base::async::IAsync::IConsumer* ) override;
         void clear_all_notifications( const base::async::IAsync::ISignature&, base::async::IAsync::IConsumer* ) override;
         bool is_subscribed( const base::async::IAsync::tSptr );
         void notify( const base::async::IAsync::tSptr );
         tConsumerMap                                 m_consumers_map;

      private:
         component::IComponent::tSptrList             m_components;
         component::IComponent::tCreatorVector        m_component_creators;

      private:
         void dump( ) const override;
   };



   inline
   const base::os::Thread& Thread::thread( ) const
   {
      return m_thread;
   }

   inline
   bool Thread::started( ) const
   {
      return m_started;
   }

   inline
   bool Thread::wait( )
   {
      m_started = m_thread.join( );
      return !m_started;
   }

} // namespace base::application
