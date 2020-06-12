#include "api/sys/dsi/Types.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "DSI_TYPES"



using namespace base::dsi;



Package::Package( )
{
}

Package::Package( const eCommand _command, const ByteBufferT& _data )
   : m_command( _command )
   , m_data( _data )
{
}

size_t Package::size( ) const
{
   return sizeof( eCommand ) + m_data.size( ) + sizeof( size_t ) /* buffer size variable */;
}

const bool Package::to_buffer( ByteBufferT& _buffer ) const
{
   return _buffer.push( m_command, m_data );
}

const bool Package::from_buffer( ByteBufferT& _buffer )
{
   return _buffer.pop( m_data, m_command );
}



Packet::Packet( )
{
}

bool Packet::to_buffer( ByteBufferT& _buffer ) const
{
   // here will be calculated CRC
   return _buffer.push( m_end_sign, m_crc, m_packages, m_size, m_begin_sign );
}

bool Packet::from_buffer( ByteBufferT& _buffer )
{
   if( false == test_buffer( _buffer ) )
      return false;

   size_t begin_sign = 0;
   size_t end_sign = 0;
   return _buffer.pop( begin_sign, m_size, m_packages, m_crc, end_sign );
}

bool Packet::test_buffer( ByteBufferT& _buffer ) const
{
   if( false == _buffer.test( m_begin_sign ) )
   {
      SYS_ERR( "begin signature mismatch" );
      return false;
   }

   size_t size = 0;
   if( false == _buffer.get( size, sizeof( m_begin_sign ) ) )
      return false;

   if( false == _buffer.test( m_end_sign, sizeof( m_begin_sign ) + size ) )
   {
      SYS_ERR( "end signature mismatch" );
      return false;
   }

   return true;
}

void Packet::add_package( const Package& _package )
{
   m_packages.push_back( _package );
   m_size += _package.size( );
}

const Package::tVector& Packet::packages( ) const
{
   return m_packages;
}
