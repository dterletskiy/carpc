#pragma once

#include <cstdlib>
#include "api/sys/common/ByteBufferT.hpp"



namespace base::dsi {

   enum class eCommand : size_t
   {
      RegisterServer,
      RegisterClient,
      UnregisterServer,
      UnregisterClient,
      BroadcastEvent,
      Undefined
   };

   class Package
   {
      public:
         using tVector = std::vector< Package >;

      public:
         Package( );
         Package( const eCommand _command, const ByteBufferT& _data );
         template< typename ... TYPES >
            Package( const eCommand _command, const TYPES& ... _values ) : m_command( _command )
            { m_data.push( _values... ); }

         size_t size( ) const;

      public:
         const bool to_buffer( ByteBufferT& ) const;
         const bool from_buffer( ByteBufferT& );

      public:
         eCommand command( ) const { return m_command; }
         const ByteBufferT& data( ) const { return m_data; }
      private:
         eCommand       m_command = eCommand::Undefined;
         ByteBufferT    m_data;
   };

   class Packet
   {
      public:
         using tSptr = std::shared_ptr< Packet >;

      public:
         Packet( );

      public:
         bool to_buffer( ByteBufferT& ) const;
         bool from_buffer( ByteBufferT& );
         bool test_buffer( ByteBufferT& ) const;

      public:
         void add_package( const Package& );
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
