#pragma once

#include "api/sys/common/ID.hpp"
#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/comm/async/IAsync.hpp"
#include "api/sys/comm/async/AsyncQueue.hpp"
#include "api/sys/comm/async/AsyncPriorityQueue.hpp"
#include "api/sys/comm/async/AsyncConsumerMap.hpp"
#include "api/sys/application/Types.hpp"



namespace base::application {

   class IThread
      : public std::enable_shared_from_this< IThread >
   {
      public:
         using tSptr = std::shared_ptr< IThread >;
         using tWptr = std::weak_ptr< IThread >;
         using tSptrList = std::list< tSptr >;

      protected:
         using tEventCollection = async::AsyncPriorityQueue;
         using tConsumerMap = async::AsyncConsumerMap;

      public:
         IThread( const std::string&, const size_t );
         virtual ~IThread( ) = default;
         IThread( const IThread& ) = delete;
         IThread& operator=( const IThread& ) = delete;

      public:
         virtual bool start( ) = 0;
         virtual void stop( ) = 0;
         virtual bool started( ) const = 0;
         virtual bool wait( ) = 0;
         virtual void boot( const std::string& ) = 0;
         virtual void shutdown( const std::string& ) = 0;
         virtual const base::os::Thread& thread( ) const = 0;
         virtual void dump( ) const = 0;

      public:
         virtual void set_notification( const base::async::IAsync::ISignature::tSptr, base::async::IAsync::IConsumer* ) = 0;
         virtual void clear_notification( const base::async::IAsync::ISignature::tSptr, base::async::IAsync::IConsumer* ) = 0;
         virtual void clear_all_notifications( const base::async::IAsync::ISignature::tSptr, base::async::IAsync::IConsumer* ) = 0;
         virtual bool insert_event( const base::async::IAsync::tSptr ) = 0;

      public:
         const thread::ID& id( ) const;
         const std::string& name( ) const;
      protected:
         thread::ID     m_id;
         std::string    m_name = "NoName";

      public:
         const size_t wd_timeout( ) const;
         const time_t process_started( ) const;
      protected:
         void process_start( );
         void process_stop( );
      private:
         size_t                        m_wd_timeout = 0;
         std::atomic< time_t >         m_process_started = 0;
   };



   inline
   const std::string& IThread::name( ) const
   {
      return m_name;
   }

   inline
   const thread::ID& IThread::id( ) const
   {
      return m_id;
   }

   inline
   const size_t IThread::wd_timeout( ) const
   {
      return m_wd_timeout;
   }

   inline
   const time_t IThread::process_started( ) const
   {
      return m_process_started.load( );
   }

   inline
   void IThread::process_start( )
   {
      return m_process_started.store( time( nullptr ) );
   }

   inline
   void IThread::process_stop( )
   {
      return m_process_started.store( 0 );
   }

} // namespace base::application
