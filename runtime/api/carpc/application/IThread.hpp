#pragma once

#include <atomic>

#include "carpc/common/ID.hpp"
#include "carpc/oswrappers/Thread.hpp"
#include "carpc/comm/async/IAsync.hpp"
#include "carpc/comm/async/AsyncQueue.hpp"
#include "carpc/comm/async/AsyncPriorityQueue.hpp"
#include "carpc/comm/async/AsyncConsumerMap.hpp"
#include "carpc/application/Context.hpp"
#include "carpc/application/Types.hpp"



namespace carpc::application {

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
         IThread( const std::string&, const std::size_t );
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
         virtual const carpc::os::Thread& thread( ) const = 0;
         virtual void dump( ) const = 0;

      public:
         virtual void set_notification( const carpc::async::IAsync::ISignature::tSptr, carpc::async::IAsync::IConsumer* ) = 0;
         virtual void clear_notification( const carpc::async::IAsync::ISignature::tSptr, carpc::async::IAsync::IConsumer* ) = 0;
         virtual void clear_all_notifications( const carpc::async::IAsync::ISignature::tSptr, carpc::async::IAsync::IConsumer* ) = 0;
         virtual bool insert_event( const carpc::async::IAsync::tSptr ) = 0;
         virtual bool send( const carpc::async::IAsync::tSptr, const application::Context& ) = 0;

      public:
         const thread::ID& id( ) const;
         const thread::Name& name( ) const;
      protected:
         thread::ID     m_id = thread::ID::generate( );
         thread::Name   m_name{ "NoName_Thread" };

      public:
         const std::size_t wd_timeout( ) const;
         const time_t process_started( ) const;
      protected:
         void process_start( );
         void process_stop( );
      private:
         std::size_t                   m_wd_timeout = 0;
         std::atomic< time_t >         m_process_started = 0;
   };



   inline
   const thread::Name& IThread::name( ) const
   {
      return m_name;
   }

   inline
   const thread::ID& IThread::id( ) const
   {
      return m_id;
   }

   inline
   const std::size_t IThread::wd_timeout( ) const
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

} // namespace carpc::application
