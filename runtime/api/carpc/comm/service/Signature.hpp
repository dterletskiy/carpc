#pragma once

#include "carpc/comm/async/Types.hpp"
#include "carpc/common/IPC.hpp"



namespace carpc::service {

   class Signature
   {
      public:
         using tVector = std::vector< Signature >;
         using tSet = std::set< Signature >;

      public:
         Signature( ) = default;
         Signature( const carpc::async::tAsyncTypeID&, const std::string& );
         Signature( const Signature& );
         ~Signature( ) = default;

      public:
         bool to_stream( carpc::ipc::tStream& ) const;
         bool from_stream( carpc::ipc::tStream& );

      public:
         bool operator==( const Signature& ) const;
         bool operator!=( const Signature& ) const;
         bool operator<( const Signature& ) const;

      public:
         const std::string dbg_name( ) const;

      public:
         const carpc::async::tAsyncTypeID& type_id( ) const;
         const std::string role( ) const;
      private:
         carpc::async::tAsyncTypeID  m_type_id;
         std::string                m_role;
   };



   inline
   const carpc::async::tAsyncTypeID& Signature::type_id( ) const
   {
      return m_type_id;
   }

   inline
   const std::string Signature::role( ) const
   {
      return m_role;
   }

} // namespace carpc::service
