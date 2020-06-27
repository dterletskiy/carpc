#pragma once

#include <cstdlib>
#include "api/sys/common/ByteStream.hpp"



namespace base::dsi {

   using tByteStream = base::ByteStream;

   enum class eCommand : size_t
   {
      RegisterServer,
      RegisterClient,
      UnregisterServer,
      UnregisterClient,
      BroadcastEvent,
      Undefined
   };
   const char* c_str( const eCommand );

   class Package
   {
      public:
         using tVector = std::vector< Package >;

      public:
         Package( );
         template< typename ... TYPES >
            Package( const eCommand command, const TYPES& ... args );
         Package( const Package& pkg ) = delete;
         Package( Package&& pkg );
         ~Package( );

      public:
         bool to_stream( tByteStream& stream ) const;
         bool from_stream( tByteStream& stream );

      public:
         size_t size( ) const;

      public:
         eCommand command( ) const;
         const RawBuffer& data( ) const;
         template< typename ... TYPES >
            bool data( TYPES& ... args ) const;
      private:
         eCommand       m_command = eCommand::Undefined;
         RawBuffer      m_data;
   };



   template< typename ... TYPES >
   Package::Package( const eCommand command, const TYPES& ... args )
      : m_command( command )
      , m_data( tByteStream::serialize( args... ) )
   {
      m_data.dump( );
   }

   inline
   size_t Package::size( ) const
   {
      return sizeof( eCommand ) + m_data.size + sizeof( size_t );
   }

   inline
   eCommand Package::command( ) const
   {
      return m_command;
   }

   inline
   const RawBuffer& Package::data( ) const
   {
      return m_data;
   }

   template< typename ... TYPES >
   bool Package::data( TYPES& ... args ) const
   {
      return tByteStream::deserialize( m_data, args... );
   }

}



namespace base::dsi {

   class Packet
   {
      public:
         using tSptr = std::shared_ptr< Packet >;

      public:
         Packet( );

      public:
         bool to_stream( tByteStream& ) const;
         bool from_stream( tByteStream& );
         bool test_buffer( tByteStream& ) const;

      public:
         void add_package( Package&& );
         template< typename ... TYPES >
            void add_package( const eCommand _command, const TYPES& ... _values )
            {
               m_packages.emplace_back(  _command, _values... );
               m_size += m_packages.back( ).size( );
            }
         const Package::tVector& packages( ) const;

      private:
         static constexpr size_t    m_begin_sign = 0xAABBCCDD;
         size_t                     m_size   = sizeof( size_t )   /* m_size */
                                             + sizeof( size_t )   /* m_packages vector count variable */
                                             + 0                  /* m_packages vector content (will be apdated during adding the package) */
                                             + sizeof( size_t );  /* m_crc */
         Package::tVector           m_packages;
         size_t                     m_crc;
         static constexpr size_t    m_end_sign = 0xFFEEDDCC;
   };

}
