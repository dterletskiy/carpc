#pragma once

#include <set>
#include <map>
#include <memory>
#include "api/sys/comm/event/Types.hpp"
#include "api/sys/dsi/Types.hpp"



namespace base::interface {

   template< typename _ID >
   struct RequestResponseIDs
   {
      using tRequestResponseIDs = RequestResponseIDs< _ID >;

      struct Comparator
      {
         struct ByRequest
         {
            bool operator( )( const tRequestResponseIDs& rr1, const tRequestResponseIDs& rr2 ) const
            {
               return rr1.request < rr2.request;
            }
         };
      };

      RequestResponseIDs( const _ID& _request )
         : request( _request ) { }
      RequestResponseIDs( const _ID& _request, const _ID& _busy, const _ID& _response )
         : request( _request ), busy( _busy ), response( _response ) { }

      _ID request = _ID::Undefined;
      _ID busy = _ID::Undefined;
      _ID response = _ID::Undefined;
   };



   template< typename _ID >
   struct NotificationIDs
   {
      _ID subscribe = _ID::Undefined;
      _ID unsubscribe = _ID::Undefined;
      _ID notification = _ID::Undefined;
   };



   struct Address
   {
      public:
         using tOpt = std::optional< Address >;
         using tSet = std::set< Address >;

      public:
         Address( ) = default;
         Address( const void*, base::ID external_id = base::InvalidID );
         Address( const Address& );
         ~Address( ) = default;

      public:
         bool to_stream( base::dsi::tByteStream& ) const;
         bool from_stream( base::dsi::tByteStream& );

      public:
         Address& operator=( const Address& );
         bool operator==( const Address& ) const;
         bool operator!=( const Address& ) const;
         bool operator<( const Address& ) const;
         operator bool( ) const;

      public:
         const std::string name( ) const;

      public:
         bool is_local( ) const;
         bool is_external( ) const;

      public:
         base::ID external_id( ) const;
         const void* ptr( ) const;
      private:
         base::ID m_external_id = base::InvalidID;
         const void* m_ptr = nullptr;
   };



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
         base::async::tAsyncTypeID m_type_id;
         std::string m_role;
   };



   using tServiceName = std::string;
   const tServiceName InvalidServiceName = "NoName";

   using tSequenceID = ID;
   const tSequenceID InvalidSequenceID = InvalidID;



   template< typename _ID >
   class TSignatureRR
   {
      public:
         TSignatureRR( ) = default;
         TSignatureRR( const tServiceName& service_name, const _ID& id, const Address& from_addr, const Address& to_addr )
            : m_service_name  ( service_name )
            , m_id            ( id )
            , m_from_addr     ( from_addr )
            , m_to_addr       ( to_addr )
         { }
         TSignatureRR( const tServiceName& service_name, const _ID& id, const Address& from_addr, const Address& to_addr, const tSequenceID seq_id )
            : m_service_name  ( service_name )
            , m_id            ( id )
            , m_from_addr     ( from_addr )
            , m_to_addr       ( to_addr )
            , m_seq_id        ( seq_id )
         { }
         TSignatureRR( const TSignatureRR& other )
            : m_service_name  ( other.m_service_name )
            , m_id            ( other.m_id )
            , m_from_addr     ( other.m_from_addr )
            , m_to_addr       ( other.m_to_addr )
            , m_seq_id        ( other.m_seq_id )
         { }
         ~TSignatureRR( ) = default;

      public:
         const std::string name( ) const
         {
            return base::format_string(
                    ", service: "      , m_service_name
                  , ", id: "           , size_t(m_id)
                  , ", from_addr: "    , m_from_addr.name( )
                  , ", to_addr: "      , m_to_addr.name( )
                  , ", seq_id: "       , m_seq_id
               );
         }

      public:
         bool operator<( const TSignatureRR< _ID >& other ) const
         {
            if( InvalidServiceName != other.m_service_name && InvalidServiceName != m_service_name )
               if( other.m_service_name != m_service_name )
                  return m_service_name < other.m_service_name;

            if( other.m_from_addr && m_from_addr )
               if( other.m_from_addr != m_from_addr )
                  return m_from_addr < other.m_from_addr;

            if( other.m_to_addr && m_to_addr )
               if( other.m_to_addr != m_to_addr )
                  return m_to_addr < other.m_to_addr;

            return m_id < other.m_id;
         }

      public:
         const bool to_stream( dsi::tByteStream& stream ) const
         {
            return stream.push( m_service_name, m_id, m_from_addr, m_to_addr, m_seq_id );
         }
         const bool from_stream( dsi::tByteStream& stream )
         {
            return stream.pop( m_service_name, m_id, m_from_addr, m_to_addr, m_seq_id );
         }

      public:
         const tServiceName& service_name( ) const       { return m_service_name; }
         const _ID id( ) const                           { return m_id; }
         const Address& from_addr( ) const               { return m_from_addr; }
         const Address& to_addr( ) const                 { return m_to_addr; }
         const tSequenceID seq_id( ) const               { return m_seq_id; }

      private:
         tServiceName         m_service_name = InvalidServiceName;
         _ID                  m_id = { };
         Address              m_from_addr;
         Address              m_to_addr;
         tSequenceID          m_seq_id = 0;
   };

} // namespace base::interface
