#pragma once

#include <set>
#include <map>
#include <memory>
#include "api/sys/common/ByteBufferT.hpp"
#include "api/sys/service/IServiceThread.hpp"



namespace base {

   template< typename _ID >
   struct RequestResponseIDs
   {
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



   using tSequenceID = size_t;
   extern const tSequenceID InvalidSequenceID;

   template< typename _ID >
   class TSignatureRR
   {
   public:
      using tID = _ID;

   public:
      TSignatureRR( ) = default;
      TSignatureRR( const std::string& service_name, const _ID& id, const void* from_addr, const void* to_addr, const tSequenceID seq_id )
         : m_service_name  ( service_name )
         , m_id            ( id )
         , mp_from_addr    ( from_addr )
         , mp_to_addr      ( to_addr )
         , m_seq_id        ( seq_id )
         , m_context       ( base::os::Thread::current_id( ) )
      { }
      TSignatureRR( const std::string& service_name, const _ID& id, const void* from_addr, const void* to_addr )
         : m_service_name  ( service_name )
         , m_id            ( id )
         , mp_from_addr    ( from_addr )
         , mp_to_addr      ( to_addr )
         , m_context       ( base::os::Thread::current_id( ) )
      { }
      TSignatureRR( const std::string& service_name, const _ID& id )
         : m_service_name  ( service_name )
         , m_id            ( id )
         , m_context       ( base::os::Thread::current_id( ) )
      { }
      TSignatureRR( const TSignatureRR& other )
         : m_service_name  ( other.m_service_name )
         , m_id            ( other.m_id )
         , mp_from_addr    ( other.mp_from_addr )
         , mp_to_addr      ( other.mp_to_addr )
         , m_seq_id        ( other.m_seq_id )
         , m_context       ( other.m_context )
      { }
      ~TSignatureRR( ) = default;

   public:
      const std::string name( ) const
      {
         return base::format_string(
                 ", service: "      , m_service_name
               , ", id: "           , size_t(m_id)
               , ", from_addr: "    , mp_from_addr
               , ", to_addr: "      , mp_to_addr
               , ", seq_id: "       , m_seq_id
               , ", context: "      , m_context
            );
      }

   public:
      bool operator<( const TSignatureRR< _ID >& signature ) const
      {
         if( signature.m_service_name != m_service_name )
            return m_service_name < signature.m_service_name;

         if( signature.mp_from_addr && mp_from_addr )
            if( signature.mp_from_addr != mp_from_addr )
               return mp_from_addr < signature.mp_from_addr;

         if( signature.mp_to_addr && mp_to_addr )
            if( signature.mp_to_addr != mp_to_addr )
               return mp_to_addr < signature.mp_to_addr;

         return m_id < signature.m_id;
      }

   public:
      const bool to_buffer( ByteBufferT& buffer ) const
      {
         return buffer.push( m_service_name, m_id, mp_from_addr, mp_to_addr, m_seq_id, m_context );
      }
      const bool from_buffer( ByteBufferT& buffer )
      {
         return buffer.pop( m_context, m_seq_id, mp_to_addr, mp_from_addr, m_id, m_service_name );
      }

   public:
      const std::string& service_name( ) const  { return m_service_name; }
      const _ID id( ) const                     { return m_id; }
      const void* from_addr( ) const            { return mp_from_addr; }
      const void* to_addr( ) const              { return mp_to_addr; }
      const tSequenceID seq_id( ) const         { return m_seq_id; }
      const TID& context( ) const               { return m_context; }

   private:
      std::string    m_service_name = { };
      _ID            m_id = { };
      const void*    mp_from_addr = nullptr;
      const void*    mp_to_addr = nullptr;
      tSequenceID    m_seq_id = 0;
      TID            m_context;
   };



} // namespace base
