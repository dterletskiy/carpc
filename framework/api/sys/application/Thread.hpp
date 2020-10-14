#pragma once

#include "api/sys/oswrappers/ConditionVariable.hpp"
#include "api/sys/comm/event/IAsync.hpp"
#include "api/sys/component/IComponent.hpp"
#include "api/sys/application/IThread.hpp"



namespace base::application {

   class Thread : public IThread
   {
      public:
         using tSptr = std::shared_ptr< Thread >;
         using tWptr = std::weak_ptr< Thread >;
         using tSptrList = std::list< tSptr >;
      private:
         struct Comparator
         {
            bool operator( )( const base::async::IAsync::ISignature* p_es1, const base::async::IAsync::ISignature* p_es2 ) const
            { return *p_es1 < *p_es2; }
         };
         using tEventConsumersMap = std::map< const base::async::IAsync::ISignature*, std::set< base::async::IAsync::IConsumer* >, Comparator >;

      public:
         struct Info
         {
            using tVector = std::vector< Info >;
            std::string                m_name;
            component::IComponent::tCreatorVector
                                       m_component_creators;
            size_t                     m_wd_timeout;
         };

      public:
         Thread( const Info& );
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
         component::IComponent::tSptrList             m_components;
         component::IComponent::tCreatorVector        m_component_creators;

      private:
         void dump( ) const;
   };



   inline
   const base::os::Thread& Thread::thread( ) const
   {
      return m_thread;
   }

   inline
   const uint64_t Thread::processed_events( ) const
   {
      return m_processed_events;
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
