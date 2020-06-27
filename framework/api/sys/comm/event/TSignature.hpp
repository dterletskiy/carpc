#pragma once

#include "api/sys/comm/event/IEvent.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TSignature"



namespace base {

   template< typename _Generator >
   class TSignature : public IEvent::ISignature
   {
      using tSignature     = typename _Generator::Config::tSignature;
      using tUserSignature = typename _Generator::Config::tUserSignature;
      using tService       = typename _Generator::Config::tService;

      public:
         static const tAsyncTypeID build_type_id( )
         {
            return tAsyncTypeID::generate< tSignature >( );
         }

      public:
         TSignature( ) = default;
         TSignature( const tUserSignature& user_signature ) : m_user_signature( user_signature ) { }
         TSignature( const TSignature& signature ) : m_user_signature( signature.m_user_signature ) { }
         ~TSignature( ) override = default;

      public:
         const IAsync::ISignature* const create_copy( ) const override
         {
            return new TSignature( *this );
         }
         const std::string name( ) const override
         {
            static const std::string s_name = format_string( "type_id: ", type_id( ).c_str( ), ", type: ", c_str( type( ) ) );
            return s_name + m_user_signature.name( );
         }
         const tAsyncTypeID& type_id( ) const override
         {
            static tAsyncTypeID _s_type_id = build_type_id( );
            return _s_type_id;
         }
         bool operator<( const IAsync::ISignature& signature ) const override
         {
            if( signature.type_id( ) != type_id( ) )
               return type_id( ) < signature.type_id( );

            if( signature.type( ) != type( ) )
               return type( ) < signature.type( );

            return m_user_signature < static_cast< const tSignature& >( signature ).m_user_signature;
         }
         const bool to_stream( dsi::tByteStream& stream ) const override
         {
            if constexpr( false == std::is_same_v< tService, NoServiceType > )
            {
               return stream.push( type_id( ), m_user_signature );
            }
            return false;
         }
         const bool from_stream( dsi::tByteStream& stream ) override
         {
            if constexpr( false == std::is_same_v< tService, NoServiceType > )
            {
               tAsyncTypeID type_id;
               return stream.pop( type_id, m_user_signature );

               // const bool result = stream.pop( type_id, m_user_signature );
               // if( type_id( ) != type_id )
               // {
               //    SYS_ERR( "event type_id mismatch" );
               // }
               // return result;
            }
            return false;
         }
         const tUserSignature& user_signature( ) const
         {
            return m_user_signature;
         }

      private:
         tUserSignature m_user_signature;
   };

} // namespace base



#undef CLASS_ABBR



#include "api/sys/oswrappers/Thread.hpp"

namespace base {

   template< typename _ID >
   class TSignatureID
   {
      public:
         using tID = _ID;

      public:
         TSignatureID( ) { }
         TSignatureID( const _ID id ) : m_id( id ) { }
         TSignatureID( const TSignatureID& signature ) : m_id( signature.m_id ) { }
         ~TSignatureID( ) = default;

      public:
         const std::string name( ) const
         {
            return base::format_string( ", id: ", size_t(m_id) );
         }
         bool operator<( const TSignatureID& signature ) const
         {
            return m_id < signature.m_id;
         }
         const bool to_stream( dsi::tByteStream& stream ) const
         {
            return stream.push( m_id );
         }
         const bool from_stream( dsi::tByteStream& stream )
         {
            return stream.pop( m_id );
         }
         const _ID id( ) const
         {
            return m_id;
         }

      public:
         _ID m_id = { };
   };

   template< typename _ID >
   class TSignatureRR
   {
      public:
         using tID = _ID;

      public:
         TSignatureRR( ) = default;
         TSignatureRR( const tServiceName& service_name, const _ID& id, const void* from_addr, const void* to_addr, const tSequenceID seq_id )
            : m_service_name  ( service_name )
            , m_id            ( id )
            , mp_from_addr    ( from_addr )
            , mp_to_addr      ( to_addr )
            , m_seq_id        ( seq_id )
            , m_context       ( base::os::Thread::current_id( ) )
         { }
         TSignatureRR( const tServiceName& service_name, const _ID& id, const void* from_addr, const void* to_addr )
            : m_service_name  ( service_name )
            , m_id            ( id )
            , mp_from_addr    ( from_addr )
            , mp_to_addr      ( to_addr )
            , m_context       ( base::os::Thread::current_id( ) )
         { }
         TSignatureRR( const tServiceName& service_name, const _ID& id )
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
            if( InvalidServiceName != signature.m_service_name && InvalidServiceName != m_service_name )
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
         const bool to_stream( dsi::tByteStream& stream ) const
         {
            return stream.push( m_service_name, m_id, mp_from_addr, mp_to_addr, m_seq_id, m_context );
         }
         const bool from_stream( dsi::tByteStream& stream )
         {
            return stream.pop( m_service_name, m_id, mp_from_addr, mp_to_addr, m_seq_id, m_context );
         }

      public:
         const tServiceName& service_name( ) const  { return m_service_name; }
         const _ID id( ) const                     { return m_id; }
         const void* from_addr( ) const            { return mp_from_addr; }
         const void* to_addr( ) const              { return mp_to_addr; }
         const tSequenceID seq_id( ) const         { return m_seq_id; }
         const TID& context( ) const               { return m_context; }

      private:
         tServiceName   m_service_name = InvalidServiceName;
         _ID            m_id = { };
         const void*    mp_from_addr = nullptr;
         const void*    mp_to_addr = nullptr;
         tSequenceID    m_seq_id = 0;
         TID            m_context;
   };

}
