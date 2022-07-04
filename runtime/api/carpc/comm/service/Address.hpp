#pragma once

#include "carpc/application/Context.hpp"
#include "carpc/comm/service/Types.hpp"
#include "carpc/common/IPC.hpp"



namespace carpc::service {

   class Address
   {
      public:
         using tOpt = std::optional< Address >;
         using tSet = std::set< Address >;

      public:
         Address( ) = default;
         Address( const comm::service::ID& );
         Address( const application::Context&, const comm::service::ID& );
         Address( const Address& );
         ~Address( ) = default;

      public:
         bool to_stream( carpc::ipc::tStream& ) const;
         bool from_stream( carpc::ipc::tStream& );

      public:
         Address& operator=( const Address& );
         bool operator==( const Address& ) const;
         bool operator!=( const Address& ) const;
         bool operator<( const Address& ) const;

      public:
         const std::string dbg_name( ) const;

      public:
         const application::Context& context( ) const;
         const comm::service::ID& id( ) const;
         bool is_valid( ) const;
      private:
         application::Context    m_context = application::Context::internal_broadcast;
         comm::service::ID       m_id = comm::service::ID::invalid;
   };



   inline
   const application::Context& Address::context( ) const
   {
      return m_context;
   }

   inline
   const comm::service::ID& Address::id( ) const
   {
      return m_id;
   }

   inline
   bool Address::is_valid( ) const
   {
      return m_id != comm::service::ID::invalid;
   }

} // namespace carpc::service
