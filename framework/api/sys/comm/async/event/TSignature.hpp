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
         static const tAsyncTypeID& build_type_id( )
         {
            static const tAsyncTypeID _s_type_id = tAsyncTypeID::generate< tSignature >( );
            return _s_type_id;
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
            static const std::string s_name = format_string( "type_id: ", type_id( ).c_str( ), ", " );
            return s_name + m_user_signature.name( );
         }
         const tAsyncTypeID& type_id( ) const override
         {
            return build_type_id( );
            // return ms_type_id;
         }
         bool operator<( const IAsync::ISignature& other ) const override
         {
            if( other.type_id( ) != type_id( ) )
               return type_id( ) < other.type_id( );

            return m_user_signature < static_cast< const tSignature& >( other ).m_user_signature;
         }
         const bool to_stream( ipc::tStream& stream ) const override
         {
            if constexpr( false == std::is_same_v< tService, NoServiceType > )
            {
               return ipc::serialize( stream, m_user_signature );
            }
            return false;
         }
         const bool from_stream( ipc::tStream& stream ) override
         {
            if constexpr( false == std::is_same_v< tService, NoServiceType > )
            {
               return ipc::deserialize( stream, m_user_signature );
            }
            return false;
         }

      public:
         const tUserSignature& user_signature( ) const
         {
            return m_user_signature;
         }
         void user_signature( const tUserSignature& signature )
         {
            m_user_signature = signature;
         }
      private:
         // static const tAsyncTypeID ms_type_id;
         tUserSignature m_user_signature;
   };

   // template< typename _Generator >
   // const tAsyncTypeID
   //    TSignature< _Generator >::ms_type_id = build_type_id( );

} // namespace base::async



#undef CLASS_ABBR



namespace base::async::simple {

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
         const bool to_stream( ipc::tStream& stream ) const
         {
            return true;
         }
         const bool from_stream( ipc::tStream& stream )
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
            if constexpr( std::is_enum_v< _ID > || std::is_integral_v< _ID > )
               return base::format_string( "id: ", std::size_t(m_id) );
            else
               return base::format_string( "id: ", m_id.name( ) );
         }
         bool operator<( const TSignature& other ) const
         {
            return m_id < other.m_id;
         }
         const bool to_stream( ipc::tStream& stream ) const
         {
            return ipc::serialize( stream, m_id );
         }
         const bool from_stream( ipc::tStream& stream )
         {
            return ipc::deserialize( stream, m_id );
         }

      public:
         const _ID id( ) const { return m_id; }

      private:
         _ID m_id = { };
   };

} // namespace base::async::id
