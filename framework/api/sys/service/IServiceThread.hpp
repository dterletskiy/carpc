#pragma once

#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/comm/event/IAsync.hpp"



namespace base {

   class IServiceThread
      : public std::enable_shared_from_this< IServiceThread >
   {
      public:
         using tSptr = std::shared_ptr< IServiceThread >;
         using tWptr = std::weak_ptr< IServiceThread >;
         using tSptrList = std::list< tSptr >;

      public:
         IServiceThread( const std::string&, const size_t );
         virtual ~IServiceThread( ) = default;
         IServiceThread( const IServiceThread& ) = delete;
         IServiceThread& operator=( const IServiceThread& ) = delete;

      public:
         virtual bool start( ) = 0;
         virtual void stop( ) = 0;
         virtual bool started( ) const = 0;
         virtual bool wait( ) = 0;
         virtual void boot( const std::string& ) = 0;
         virtual void shutdown( const std::string& ) = 0;
         virtual const base::os::Thread& thread( ) const = 0;

      public:
         virtual void set_notification( const base::async::IAsync::ISignature&, base::async::IAsync::IConsumer* ) = 0;
         virtual void clear_notification( const base::async::IAsync::ISignature&, base::async::IAsync::IConsumer* ) = 0;
         virtual void clear_all_notifications( const base::async::IAsync::ISignature&, base::async::IAsync::IConsumer* ) = 0;
         virtual bool insert_event( const base::async::IAsync::tSptr ) = 0;

      public:
         const ID& id( ) const;
         const std::string& name( ) const;
      protected:
         ID             m_id = InvalidID;
         std::string    m_name = "NoName";

      public:
         const size_t wd_timeout( ) const;
         const std::optional< time_t > process_started( ) const;
      protected:
         size_t                        m_wd_timeout = 0;
         std::optional< time_t >       m_process_started = std::nullopt;
   };



   inline
   const std::string& IServiceThread::name( ) const
   {
      return m_name;
   }

   inline
   const ID& IServiceThread::id( ) const
   {
      return m_id;
   }

   inline
   const size_t IServiceThread::wd_timeout( ) const
   {
      return m_wd_timeout;
   }

   inline
   const std::optional< time_t > IServiceThread::process_started( ) const
   {
      return m_process_started;
   }

} // namespace base
