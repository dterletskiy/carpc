#pragma once

#include "api/sys/comm/service/Types.hpp"



namespace carpc::service::fast {

   template< typename _ID >
   class TSignature
   {
      public:
         using tID = _ID;

      public:
         TSignature( ) = default;
         TSignature( const comm::service::Name&, const _ID&, const comm::service::ID& from = { }, const comm::service::ID to = { }, const comm::sequence::ID seq_id = { } );
         TSignature( const TSignature& other );
         ~TSignature( ) = default;

      public:
         const std::string name( ) const;
         bool operator<( const TSignature& ) const;
         const bool to_stream( ipc::tStream& ) const;
         const bool from_stream( ipc::tStream& );

      public:
         const comm::service::Name& role( ) const;
         const _ID& id( ) const;
         const comm::service::ID& from( ) const;
         const comm::service::ID& to( ) const;
         const comm::sequence::ID& seq_id( ) const;

      private:
         comm::service::Name m_role = comm::service::Name::invalid;
         _ID m_id = { };
         comm::service::ID m_from;
         comm::service::ID m_to;
         comm::sequence::ID m_seq_id = comm::sequence::ID::invalid;
   };



   template< typename _ID >
   TSignature< _ID >::TSignature(
               const comm::service::Name& role, const _ID& id,
               const comm::service::ID& from, const comm::service::ID to,
               const comm::sequence::ID seq_id
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
      return carpc::format_string(
         "role: ", m_role.value( ),
         ", id: ", m_id.c_str( ),
         ", from: ", m_from.name( ),
         ", to: ", m_to.name( ),
         ", seq_id: ", m_seq_id.name( )
      );
   }

   template< typename _ID >
   bool TSignature< _ID >::operator<( const TSignature& other ) const
   {
      if( m_from.is_valid( ) && other.m_from.is_valid( ) )
         if( m_from != other.m_from )
            return m_from < other.m_from;
      if( m_to.is_valid( ) && other.m_to.is_valid( ) )
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
   const comm::service::Name& TSignature< _ID >::role( ) const
   {
      return m_role;
   }

   template< typename _ID >
   const _ID& TSignature< _ID >::id( ) const
   {
      return m_id;
   }

   template< typename _ID >
   const comm::service::ID& TSignature< _ID >::from( ) const
   {
      return m_from;
   }

   template< typename _ID >
   const comm::service::ID& TSignature< _ID >::to( ) const
   {
      return m_to;
   }

   template< typename _ID >
   const comm::sequence::ID& TSignature< _ID >::seq_id( ) const
   {
      return m_seq_id;
   }

} // namespace carpc::service::fast
