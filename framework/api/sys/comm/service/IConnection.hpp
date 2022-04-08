#pragma once

#include "api/sys/comm/async/event/Event.hpp"
#include "api/sys/comm/async/runnable/Runnable.hpp"
#include "api/sys/application/Context.hpp"
#include "api/sys/events/Events.hpp"
#include "api/sys/comm/service/Types.hpp"



namespace carpc::service {

   class Address;



   class IConnection : public events::service::Status::Consumer
   {
      public:
         enum class eType: std::uint8_t { Server, Proxy, Undefined };
      public:
         IConnection( const carpc::async::tAsyncTypeID&, const std::string&, const bool, const eType );
         virtual ~IConnection( );

      protected:
         enum class eStatus: std::uint8_t { Connected, Disconnected };

      protected:
         virtual void status( const Address&, const eStatus ) = 0;
         virtual void connected( const Address& ) = 0;
         virtual void disconnected( const Address& ) = 0;
         virtual void connected( ) = 0;
         virtual void disconnected( ) = 0;
         virtual const bool is_connected( ) const = 0;

      private:
         void process_event( const events::service::Status::Event& ) override final;

      public:
         const Passport passport( ) const;

      public:
         const Signature& signature( ) const;
      private:
         Signature               m_signature;

      public:
         const application::Context& context( ) const;
      protected:
         application::Context    m_context = application::Context::current( );

      public:
         const comm::service::ID& id( ) const;
      protected:
         comm::service::ID       m_id = comm::service::ID::generate( );

      public:
         bool is_external( ) const;
      protected:
         bool                    m_is_external = false;

      public:
         eType type( ) const;
      protected:
         eType                   m_type = eType::Undefined;
   };



   inline
   const Passport IConnection::passport( ) const
   {
      return Passport( m_signature, Address( m_context, m_id ) );
   }

   inline
   const Signature& IConnection::signature( ) const
   {
      return m_signature;
   }

   inline
   const application::Context& IConnection::context( ) const
   {
      return m_context;
   }

   inline
   const comm::service::ID& IConnection::id( ) const
   {
      return m_id;
   }

   inline
   bool IConnection::is_external( ) const
   {
      // @TDA: here should be check of combination is_export and is_ipc
      return m_is_external;
   }

   inline
   IConnection::eType IConnection::type( ) const
   {
      return m_type;
   }

} // namespace carpc::service
