#include "api/sys/dsi/Types.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "DSI_TYPES"



namespace base::dsi {

   const char* c_str( const eCommand comm_type )
   {
      switch( comm_type )
      {
         case eCommand::RegisterServer:      return "RegisterServer";
         case eCommand::RegisterClient:      return "RegisterClient";
         case eCommand::UnregisterServer:    return "UnregisterServer";
         case eCommand::UnregisterClient:    return "UnregisterClient";
         case eCommand::BroadcastEvent:      return "BroadcastEvent";
         default:                            return "Undefined";
      }
      return "Undefined";
   }

}


using namespace base::dsi;



Package::Package( )
{
}

Package::Package( Package&& pkg )
   : m_command( pkg.m_command )
{
   SYS_WRN( "move" );
   m_data = pkg.m_data;
   pkg.m_data = { };
}

Package::~Package( )
{
   m_data.free( );
}

bool Package::to_stream( tByteStream& _stream ) const
{
   return _stream.push( m_command, m_data );
}

bool Package::from_stream( tByteStream& _stream )
{
   return _stream.pop( m_command, m_data );
}



Packet::Packet( )
{
}

bool Packet::to_stream( tByteStream& _stream ) const
{
   // here will be calculated CRC
   return _stream.push( m_begin_sign, m_size, m_packages, m_crc, m_end_sign );
}

bool Packet::from_stream( tByteStream& _stream )
{
   if( false == test_buffer( _stream ) )
      return false;

   size_t begin_sign = 0;
   size_t end_sign = 0;
   return _stream.pop( begin_sign, m_size, m_packages, m_crc, end_sign );
}

bool Packet::test_buffer( tByteStream& _stream ) const
{
   // if( false == _stream.test( m_begin_sign ) )
   // {
   //    SYS_ERR( "begin signature mismatch" );
   //    return false;
   // }

   // size_t size = 0;
   // if( false == _stream.get( size, sizeof( m_begin_sign ) ) )
   //    return false;

   // if( false == _stream.test( m_end_sign, sizeof( m_begin_sign ) + size ) )
   // {
   //    SYS_ERR( "end signature mismatch" );
   //    return false;
   // }

   return true;
}

void Packet::add_package( Package&& _package )
{
   m_packages.emplace_back( std::move( _package ) );
   m_size += _package.size( );
}

const Package::tVector& Packet::packages( ) const
{
   return m_packages;
}
