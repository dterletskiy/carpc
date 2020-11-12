#include "api/sys/helpers/functions/format.hpp"
#include "api/sys/dsi/Types.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "DSI_TYPES"



namespace base::dsi {

   const char* c_str( const eCommand comm_type )
   {
      switch( comm_type )
      {
         case eCommand::RegisterServer:         return "RegisterServer";
         case eCommand::UnregisterServer:       return "UnregisterServer";
         case eCommand::RegisterClient:         return "RegisterClient";
         case eCommand::UnregisterClient:       return "UnregisterClient";
         case eCommand::DetectedServer:         return "DetectedServer";
         case eCommand::DetectedClient:         return "DetectedClient";
         case eCommand::RegisterProcess:        return "RegisterProcess";
         case eCommand::RegisterProcessAck:     return "RegisterProcessAck";
         case eCommand::RegisterClientProcess:  return "RegisterClientProcess";
         case eCommand::RegisterServerProcess:  return "RegisterServerProcess";
         case eCommand::BroadcastEvent:         return "BroadcastEvent";
         default:                               return "Undefined";
      }
      return "Undefined";
   }

}


using namespace base::dsi;



const std::string SocketCongiguration::name( ) const
{
   return base::format_string( domain, ".", type, ".", protocole, "-", address, ":", port );
}

bool SocketCongiguration::to_stream( tByteStream& stream ) const
{
   return stream.push( domain, type, protocole, address, port );
}

bool SocketCongiguration::from_stream( tByteStream& stream )
{
   return stream.pop( domain, type, protocole, address, port );
}



Package::Package( )
{
}

Package::Package( Package&& pkg )
   : m_command( pkg.m_command )
   , m_data( pkg.m_data )
{
}

Package::~Package( )
{
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

Packet::~Packet( )
{
}

bool Packet::to_stream( tByteStream& _stream ) const
{
   // here will be calculated CRC
   return _stream.push( m_begin_sign, m_size, m_packages, m_crc, m_end_sign );
}

bool Packet::from_stream( tByteStream& _stream )
{
   // if( false == test_stream( _stream ) )
   //    return false;

   size_t begin_sign = 0;
   size_t end_sign = 0;
   return _stream.pop( begin_sign, m_size, m_packages, m_crc, end_sign );
}

bool Packet::test_stream( tByteStream& _stream ) const
{
   // size_t begin_sign = 0;
   // if( false == _stream.get( 0, begin_sign ) )
   // {
   //    SYS_ERR( "begin signature read error" );
   //    return false;
   // }
   // if( m_begin_sign != begin_sign )
   // {
   //    SYS_ERR( "begin signature mismatch" );
   //    return false;
   // }

   // size_t size = 0;
   // if( false == _stream.get( sizeof( m_begin_sign ), size ) )
   // {
   //    SYS_ERR( "size read error" );
   //    return false;
   // }

   // size_t end_sign = 0;
   // if( false == _stream.get( sizeof( m_begin_sign ) + size, end_sign ) )
   // {
   //    SYS_ERR( "end signature read error" );
   //    return false;
   // }
   // if( m_end_sign != end_sign )
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
