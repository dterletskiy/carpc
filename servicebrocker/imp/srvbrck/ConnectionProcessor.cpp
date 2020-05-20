#include "api/sys/oswrappers/linux/kernel.hpp"
#include "ConnectionProcessor.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "ConnProc"



namespace {

   const size_t s_buffer_size = base::configuration::dsi::buffer_size;
   // const size_t s_buffer_size = 10;

}

ConnectionProcessor::ConnectionProcessor( )
{
   mp_buffer = malloc( s_buffer_size );
}

ConnectionProcessor::~ConnectionProcessor( )
{
   free( mp_buffer );
}

bool ConnectionProcessor::init( const int _socket_family, const int _socket_type, const int _socket_protocole, const char* _server_address, const int _server_port )
{
   unlink( _server_address );

   m_master_socket = base::os::linux::socket::socket( _socket_family, _socket_type, _socket_protocole );
   if( -1 == m_master_socket )
      return false;

   if( false == base::os::linux::socket::bind( m_master_socket, _socket_family, _server_address, _server_port ) )
      return false;

   base::os::linux::set_nonblock( m_master_socket );

   if( false == base::os::linux::socket::listen( m_master_socket ) )
      return false;

   base::os::linux::socket::info( m_master_socket, "Connection created" );

   return true;
}

bool ConnectionProcessor::shutdown( )
{
   return true;
}

void ConnectionProcessor::fd_set_reset( )
{
   FD_ZERO( &m_fd_set_read );
   FD_ZERO( &m_fd_set_write );
   FD_ZERO( &m_fd_set_except );
}

base::os::linux::socket::tSocket ConnectionProcessor::fd_set_init( )
{
   FD_SET( m_master_socket, &m_fd_set_read );
   base::os::linux::socket::tSocket max_socket = m_master_socket;
   for( const auto& slave_socket : m_slave_sockets_set )
   {
      FD_SET( slave_socket, &m_fd_set_read );
      if( slave_socket > max_socket )
         max_socket = slave_socket;
   }
   return max_socket;
}

void ConnectionProcessor::connection_loop( )
{
   while( true )
   {
      fd_set_reset( );

      if( false == base::os::linux::socket::select( fd_set_init( ), &m_fd_set_read ) )
         continue;

      process_slave_sockets( );

      if( FD_ISSET( m_master_socket, &m_fd_set_read ) )
      {
         base::os::linux::socket::tSocket slave_socket = base::os::linux::socket::accept( m_master_socket );
         base::os::linux::set_nonblock( slave_socket );
         m_slave_sockets_set.insert( slave_socket );

         base::os::linux::socket::info( slave_socket, "Host connected" );
      }
   }
}

void ConnectionProcessor::process_slave_sockets( )
{
   std::set< base::os::linux::socket::tSocket > slave_sockets_to_remove_set;
   for( const auto& slave_socket : m_slave_sockets_set )
   {
      if( !FD_ISSET( slave_socket, &m_fd_set_read ) ) continue;
      if( eRead::DISCONNECTED == read_slave_socket( slave_socket ) )
      {
         slave_sockets_to_remove_set.insert( slave_socket );
         base::os::linux::socket::close( slave_socket );
      }
   }
   for( const auto& slave_socket : slave_sockets_to_remove_set )
      m_slave_sockets_set.erase( slave_socket );
}

ConnectionProcessor::eRead ConnectionProcessor::read_slave_socket( const base::os::linux::socket::tSocket slave_socket )
{
   ssize_t read_size = base::os::linux::socket::recv( slave_socket, mp_buffer, s_buffer_size );
   if( 0 >= read_size )
   {
      if( base::os::linux::socket::error != EAGAIN )
      {
         base::os::linux::socket::info( slave_socket, "Host disconnected" );
         return eRead::DISCONNECTED;
      }
      return eRead::ERROR;
   }





   base::os::linux::socket::send( slave_socket, mp_buffer, read_size );

   return eRead::OK;
}
