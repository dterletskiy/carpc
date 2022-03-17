#pragma once

#include <atomic>

#include "api/sys/application/IComponent.hpp"
#include "api/sys/application/IThread.hpp"



namespace base::application {

   class Thread : public IThread
   {
      public:
         struct Configuration
         {
            using tVector = std::vector< Configuration >;
            std::string                m_name;
            IComponent::tCreatorVector m_component_creators;
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
         std::atomic< bool >                          m_started = false;

      private:
         bool insert_event( const base::async::IAsync::tSptr ) override;
         bool send( const base::async::IAsync::tSptr, const application::Context& ) override;
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
         IComponent::tSptrList                        m_components;
         IComponent::tCreatorVector                   m_component_creators;

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
      return m_started.load( );
   }

   inline
   bool Thread::wait( )
   {
      const bool started = m_thread.join( );
      m_started.store( started );
      return !m_started.load( );
   }

} // namespace base::application
