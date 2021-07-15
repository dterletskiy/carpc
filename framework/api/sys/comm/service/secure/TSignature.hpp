#pragma once

#include "api/sys/comm/service/Types.hpp"



namespace base::service::secure {

   template< typename _ID >
   class TSignature
   {
      public:
         using tID = _ID;

      public:
         TSignature( ) = default;
         TSignature( const tServiceName&, const _ID&, const service::ID& from = { }, const service::ID to = { }, const tSequenceID seq_id = { } );
         TSignature( const TSignature& other );
         ~TSignature( ) = default;

      public:
         const std::string name( ) const;
         bool operator<( const TSignature& ) const;
         const bool to_stream( ipc::tStream& ) const;
         const bool from_stream( ipc::tStream& );

      public:
         const tServiceName& role( ) const;
         const _ID& id( ) const;
         const service::ID& from( ) const;
         const service::ID& to( ) const;
         const tSequenceID& seq_id( ) const;

      private:
         tServiceName m_role = InvalidServiceName;
         _ID m_id = { };
         service::ID m_from;
         service::ID m_to;
         tSequenceID m_seq_id = tSequenceID::invalid;
   };



   template< typename _ID >
   TSignature< _ID >::TSignature(
               const tServiceName& role, const _ID& id,
               const service::ID& from, const service::ID to,
               const tSequenceID seq_id
   )
      : m_role( role )
      , m_id( id )
      , m_from( from )
      , m_to( to )
      , m_seq_id( seq_id )
   {
   }

   template< typename _ID >
   TSignature< _ID >::TSignature( const TSignature& other )
      : m_role( other.m_role )
      , m_id( other.m_id )
      , m_from( other.m_from )
      , m_to( other.m_to )
      , m_seq_id( other.m_seq_id )
   {
   }

   template< typename _ID >
   const std::string TSignature< _ID >::name( ) const
   {
      return base::format_string(
         "role: ", m_role,
         ", id: ", m_id.c_str( ),
         ", from: ", m_from.name( ),
         ", to: ", m_to.name( ),
         ", seq_id: ", m_seq_id.name( )
      );
   }

   template< typename _ID >
   bool TSignature< _ID >::operator<( const TSignature& other ) const
   {
      if( m_from != other.m_from )
         return m_from < other.m_from;
      if( m_to != other.m_to )
         return m_to < other.m_to;
      if( m_role != other.m_role )
         return m_role < other.m_role;
      return m_id < other.m_id;
   }

   template< typename _ID >
   const bool TSignature< _ID >::to_stream( ipc::tStream& stream ) const
   {
      return ipc::serialize( stream, m_role, m_id, m_from, m_to, m_seq_id );
   }

   template< typename _ID >
   const bool TSignature< _ID >::from_stream( ipc::tStream& stream )
   {
      return ipc::deserialize( stream, m_role, m_id, m_from, m_to, m_seq_id );
   }

   template< typename _ID >
   const tServiceName& TSignature< _ID >::role( ) const
   {
      return m_role;
   }

   template< typename _ID >
   const _ID& TSignature< _ID >::id( ) const
   {
      return m_id;
   }

   template< typename _ID >
   const service::ID& TSignature< _ID >::from( ) const
   {
      return m_from;
   }

   template< typename _ID >
   const service::ID& TSignature< _ID >::to( ) const
   {
      return m_to;
   }

   template< typename _ID >
   const tSequenceID& TSignature< _ID >::seq_id( ) const
   {
      return m_seq_id;
   }

} // namespace base::service::secure
