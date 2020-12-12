#pragma once

#include "api/sys/comm/service/Types.hpp"



namespace base::service::fast {

   template< typename _ID >
   class TSignature
   {
      public:
         using tID = _ID;

      public:
         TSignature( ) = default;
         TSignature( const tServiceName& role, const _ID& id, const service::ID& from = { }, const service::ID to = { }, const tSequenceID seq_id = { } )
            : m_role( role )
            , m_id( id )
            , m_from( from )
            , m_to( to )
            , m_seq_id( seq_id )
         { }
         TSignature( const TSignature& other )
            : m_role( other.m_role )
            , m_id( other.m_id )
            , m_from( other.m_from )
            , m_to( other.m_to )
            , m_seq_id( other.m_seq_id )
         { }
         ~TSignature( ) = default;

      public:
         const std::string name( ) const
         {
            return base::format_string(
               "role: ", m_role,
               ", id: ", size_t(m_id),
               ", from: ", m_from.name( ),
               ", to: ", m_to.name( ),
               ", seq_id: ", m_seq_id.name( )
            );
         }
         bool operator<( const TSignature& other ) const
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
         const bool to_stream( dsi::tByteStream& stream ) const
         {
            return stream.push( m_role, m_id, m_from, m_to, m_seq_id );
         }
         const bool from_stream( dsi::tByteStream& stream )
         {
            return stream.pop( m_role, m_id, m_from, m_to, m_seq_id );
         }

      public:
         const tServiceName& role( ) const { return m_role; }
         const _ID& id( ) const { return m_id; }
         const service::ID& from( ) const { return m_from; }
         const service::ID& to( ) const { return m_to; }
         const tSequenceID& seq_id( ) const { return m_seq_id; }

      private:
         tServiceName m_role = InvalidServiceName;
         _ID m_id = { };
         service::ID m_from;
         service::ID m_to;
         tSequenceID m_seq_id = tSequenceID::invalid( );
   };

} // namespace base::service::fast
