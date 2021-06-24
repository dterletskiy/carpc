#pragma once

#include "api/sys/common/ID.hpp"
#include "api/sys/common/Priority.hpp"
#include "api/sys/oswrappers/Types.hpp"
#include "api/sys/oswrappers/linux/socket.hpp"
#include "api/sys/oswrappers/linux/timer.hpp"
#include "api/sys/comm/service/Registry.hpp"
#include "api/sys/application/Thread.hpp"
#include "api/sys/application/Types.hpp"
#include "api/sys/tools/Tools.hpp"



namespace base::application {

   class Process
      : public std::enable_shared_from_this< Process >
   {
      public:
         using tSptr = std::shared_ptr< Process >;
         using tWptr = std::weak_ptr< Process >;
         using tUptr = std::unique_ptr< Process >;
         using tRptr = Process*;

      public:
         static Process& invalid( )
         {
            static Process process( 0, nullptr, nullptr );
            return process;
         }
         bool operator==( const Process& other ) const { return this == &other; }

      public:
         ~Process( );
         static tSptr instance( int argc = 0, char** argv = nullptr, char** envp = nullptr );
      private:
         Process( int argc, char** argv, char** envp );
         Process( const Process& ) = delete;
         Process& operator=( const Process& ) = delete;
         static tSptr mp_instance;

      public:
         bool start( const Thread::Configuration::tVector& );
         bool stop( );
         void boot( );

      public:
         const process::ID& id( ) const;
         const std::string& name( ) const;
      private:
         process::ID    m_id;
         std::string    m_name = "NoName";

      public:
         IThread::tSptr thread_ipc( ) const;
      private:
         IThread::tSptr   mp_thread_ipc = nullptr;

      public:
         IThread::tSptr thread( const thread::ID& ) const;
         IThread::tSptr current_thread( ) const;
         Context current_context( ) const;
         IThread::tSptrList thread_list( ) const;
      private:
         IThread::tSptrList  m_thread_list;

      public:
         service::Registry& service_registry( );
      private:
         service::Registry m_service_registry;

      private:
         os::os_linux::timer::tID      m_timer_id;

      public:
         struct Configuration
         {
            struct IPC
            {
               os::os_linux::socket::configuration socket;
               std::size_t                         buffer_size;
            };
            IPC ipc_sb;
            IPC ipc_app;

            const tPriority max_priority = priority( ePriority::MAX );
         };
      public:
         const Configuration& configuration( ) const;
      private:
         Configuration m_configuration;

      public:
         const tools::PCE& pce( ) const;
      private:
         tools::PCE m_pce;
   };



   inline
   const std::string& Process::name( ) const
   {
      return m_name;
   }

   inline
   const process::ID& Process::id( ) const
   {
      return m_id;
   }

   inline
   IThread::tSptrList Process::thread_list( ) const
   {
      return m_thread_list;
   }

   inline
   service::Registry& Process::service_registry( )
   {
      return m_service_registry;
   }

   inline
   const Process::Configuration& Process::configuration( ) const
   {
      return m_configuration;
   }

   inline
   const tools::PCE& Process::pce( ) const
   {
      return m_pce;
   }

} // namespace base::application
