#pragma once

#include "api/sys/comm/event/Types.hpp"
#include "api/sys/dsi/Types.hpp"



namespace base::service {

   class Signature
   {
      public:
         Signature( ) = default;
         Signature( const base::async::tAsyncTypeID&, const std::string& );
         Signature( const Signature& );
         ~Signature( ) = default;

      public:
         bool to_stream( base::dsi::tByteStream& ) const;
         bool from_stream( base::dsi::tByteStream& );

      public:
         Signature& operator=( const Signature& );
         bool operator==( const Signature& ) const;
         bool operator!=( const Signature& ) const;
         bool operator<( const Signature& ) const;

      public:
         const std::string name( ) const;

      public:
         const base::async::tAsyncTypeID& type_id( ) const;
         const std::string role( ) const;
      private:
         base::async::tAsyncTypeID  m_type_id;
         std::string                m_role;
   };



   inline
   const base::async::tAsyncTypeID& Signature::type_id( ) const
   {
      return m_type_id;
   }

   inline
   const std::string Signature::role( ) const
   {
      return m_role;
   }

} // namespace base::service
