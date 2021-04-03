#pragma once

#include "api/sys/application/Context.hpp"
#include "api/sys/comm/service/Types.hpp"
#include "api/sys/common/IPC.hpp"



namespace base::service {

   class Address
   {
      public:
         using tOpt = std::optional< Address >;
         using tSet = std::set< Address >;

      public:
         Address( ) = default;
         Address( const ID& );
         Address( const application::Context&, const ID& );
         Address( const Address& );
         ~Address( ) = default;

      public:
         bool to_stream( base::ipc::tStream& ) const;
         bool from_stream( base::ipc::tStream& );

      public:
         Address& operator=( const Address& );
         bool operator==( const Address& ) const;
         bool operator!=( const Address& ) const;
         bool operator<( const Address& ) const;

      public:
         const std::string name( ) const;

      public:
         const application::Context& context( ) const;
         const ID& id( ) const;
         bool is_valid( ) const;
      private:
         application::Context    m_context = application::Context::internal_broadcast;
         ID                      m_id = ID::invalid;
   };



   inline
   const application::Context& Address::context( ) const
   {
      return m_context;
   }

   inline
   const ID& Address::id( ) const
   {
      return m_id;
   }

   inline
   bool Address::is_valid( ) const
   {
      return m_id != ID::invalid;
   }

} // namespace base::service
