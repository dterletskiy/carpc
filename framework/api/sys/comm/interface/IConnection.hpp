#pragma once

#include <string>

#include "api/sys/service/IServiceThread.hpp"
#include "api/sys/comm/event/Event.hpp"
#include "api/sys/comm/event/Runnable.hpp"
#include "api/sys/comm/interface/Types.hpp"
#include "api/sys/events/Events.hpp"



namespace base::interface {

   class IConnection : public events::interface::Status::Consumer
   {
      public:
         IConnection( const base::async::tAsyncTypeID&, const std::string&, const bool );
         virtual ~IConnection( );

      public:
         virtual void connected( const Address& ) = 0;
         virtual void disconnected( const Address& ) = 0;
         virtual void connected( ) = 0;
         virtual void disconnected( ) = 0;
         virtual const bool is_connected( ) const = 0;

      private:
         void process_event( const events::interface::Status::Event& ) override;

      public:
         bool is_external( ) const;
      protected:
         bool                    m_is_external = false;

      public:
         const Signature& signature( ) const;
      private:
         Signature               m_signature;

      public:
         IServiceThread::tWptr context( ) const;
      private:
         IServiceThread::tWptr   mp_service_thread;
   };



   inline
   bool IConnection::is_external( ) const
   {
      return m_is_external;
   }

   inline
   const Signature& IConnection::signature( ) const
   {
      return m_signature;
   }

   inline
   IServiceThread::tWptr IConnection::context( ) const
   {
      return mp_service_thread;
   }

} // namespace base::interface
