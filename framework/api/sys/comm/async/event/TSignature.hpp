#pragma once

#include "api/sys/comm/async/event/IEvent.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TSignature"



namespace base::async {

   template< typename _Generator >
   class TSignature : public IEvent::ISignature
   {
      using tSignature     = typename _Generator::Config::tSignature;
      using tUserSignature = typename _Generator::Config::tUserSignature;
      using tService       = typename _Generator::Config::tService;

      public:
         using tSptr       = std::shared_ptr< tSignature >;

      public:
         static const tAsyncTypeID build_type_id( )
         {
            return tAsyncTypeID::generate< tSignature >( );
         }

      private:
         TSignature( ) = default;
         TSignature( const tUserSignature& user_signature )
            : m_user_signature( user_signature )
         { }
         TSignature( const TSignature& other )
            : m_user_signature( other.m_user_signature )
         { }
      public:
         ~TSignature( ) override = default;
         static tSptr create( const tUserSignature& user_signature = { } )
         {
            return tSptr( new tSignature( user_signature ) );
         }

      public:
         const std::string name( ) const override
         {
            static const std::string s_name = format_string( "type_id: ", type_id( ).c_str( ), ", type: ", c_str( type( ) ), ", " );
            return s_name + m_user_signature.name( );
         }
         const tAsyncTypeID& type_id( ) const override
         {
            static tAsyncTypeID _s_type_id = build_type_id( );
            return _s_type_id;
         }
         bool operator<( const IAsync::ISignature& other ) const override
         {
            if( other.type_id( ) != type_id( ) )
               return type_id( ) < other.type_id( );

            if( other.type( ) != type( ) )
               return type( ) < other.type( );

            return m_user_signature < static_cast< const tSignature& >( other ).m_user_signature;
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
               const bool result = stream.pop( type_id, m_user_signature );
               // if( type_id( ) != type_id ) { SYS_ERR( "event type_id mismatch" ); }
               return result;
            }
            return false;
         }

      public:
         const tUserSignature& user_signature( ) const { return m_user_signature; }
         void user_signature( const tUserSignature& signature ) { m_user_signature = signature; }
      private:
         tUserSignature m_user_signature;
   };

} // namespace base::async



#undef CLASS_ABBR



namespace base::async::simple {

   template< typename _ID >
   class Signature
   {
      public:
         Signature( ) = default;
         Signature( const Signature& other ) = default;
         ~Signature( ) = default;

      public:
         const std::string name( ) const
         {
            return std::string{ };
         }
         bool operator<( const Signature& other ) const
         {
            return false;
         }
         const bool to_stream( dsi::tByteStream& stream ) const
         {
            return true;
         }
         const bool from_stream( dsi::tByteStream& stream )
         {
            return true;
         }
   };

} // namespace base::async::simple

namespace base::async::id {

   template< typename _ID >
   class TSignature
   {
      public:
         TSignature( ) = default;
         TSignature( const _ID id ) : m_id( id ) { }
         TSignature( const TSignature& other ) : m_id( other.m_id ) { }
         ~TSignature( ) = default;

      public:
         const std::string name( ) const
         {
            return base::format_string( "id: ", size_t(m_id) );
         }
         bool operator<( const TSignature& other ) const
         {
            return m_id < other.m_id;
         }
         const bool to_stream( dsi::tByteStream& stream ) const
         {
            return stream.push( m_id );
         }
         const bool from_stream( dsi::tByteStream& stream )
         {
            return stream.pop( m_id );
         }

      public:
         const _ID id( ) const { return m_id; }

      private:
         _ID m_id = { };
   };

} // namespace base::async::id
