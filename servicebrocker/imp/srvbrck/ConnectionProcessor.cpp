#include "api/sys/oswrappers/linux/kernel.hpp"
#include "ConnectionProcessor.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "ConnProc"



namespace {
   void print_socket( const int _socket, const char* _message )
   {
      struct sockaddr_in address;
      socklen_t addrlen = sizeof( address );
      getpeername( _socket , (struct sockaddr*)&address , (socklen_t*)&addrlen );
      SYS_INF( "%s, ip:port/socket - %s:%d/%d ", _message, inet_ntoa(address.sin_addr), ntohs(address.sin_port), _socket );
   }
}



ConnectionProcessor::ConnectionProcessor( )
{
   mp_buffer = malloc( base::configuration::dsi::buffer_size );
}

ConnectionProcessor::~ConnectionProcessor( )
{
   free( mp_buffer );
}

bool ConnectionProcessor::init( const int _socket_family, const int _socket_type, const int _socket_protocole, const char* _server_address, const int _server_port )
{
   m_master_socket = socket( _socket_family, _socket_type, _socket_protocole );
   if( -1 == m_master_socket )
   {
      m_last_errno = errno;
      SYS_ERR( "socket(%d) error: %d", m_master_socket, m_last_errno );
      return false;
   }
   SYS_MSG( "socket(%d)", m_master_socket );

   sockaddr* p_sockaddr = nullptr;
   size_t sockaddr_size = 0;
   struct sockaddr_un serv_addr_un;
   struct sockaddr_in serv_addr_in;
   if( AF_UNIX == _socket_family )
   {
      SYS_INF( "AF_UNIX" );
      memset( &serv_addr_un, 0, sizeof( serv_addr_un ) );
      serv_addr_un.sun_family = _socket_family;
      strncpy( serv_addr_un.sun_path, _server_address, sizeof( serv_addr_un.sun_path ) - 1 );
      unlink( _server_address );
      sockaddr_size = sizeof( serv_addr_un.sun_family ) + strlen( serv_addr_un.sun_path );

      p_sockaddr = reinterpret_cast< sockaddr* >( &serv_addr_un );
   }
   else if( AF_INET == _socket_family )
   {
      SYS_INF( "AF_INET" );
      memset( &serv_addr_in, 0, sizeof( serv_addr_in ) );
      serv_addr_in.sin_family = _socket_family;
      serv_addr_in.sin_addr.s_addr = inet_addr( _server_address );
      // serv_addr_in.sin_addr.s_addr = htonl( INADDR_ANY );
      // serv_addr_in.sin_addr.s_addr = htonl( INADDR_LOOPBACK );
      serv_addr_in.sin_port = htons( _server_port );
      sockaddr_size = sizeof( serv_addr_in );

      p_sockaddr = reinterpret_cast< sockaddr* >( &serv_addr_in );
   }

   int result_bind = bind( m_master_socket, p_sockaddr, sockaddr_size );
   if( -1 == result_bind )
   {
      m_last_errno = errno;
      SYS_ERR( "bind(%d) error: %d", m_master_socket, m_last_errno );
      return false;
   }
   SYS_MSG( "bind(%d)", m_master_socket );

   base::os::linux::set_nonblock( m_master_socket );

   int result_listen = listen( m_master_socket, SOMAXCONN );
   if( -1 == result_listen )
   {
      m_last_errno = errno;
      SYS_ERR( "listen(%d) error: %d", m_master_socket, m_last_errno );
      return false;
   }
   SYS_MSG( "listen(%d)", m_master_socket );

   print_socket( m_master_socket, "Connection created" );

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

void ConnectionProcessor::connection_loop( )
{
   while( true )
   {
      fd_set_reset( );

      FD_SET( m_master_socket, &m_fd_set_read );
      int max_socket = m_master_socket;
      for( const auto& slave_socket : m_slave_sockets_set )
      {
         FD_SET( slave_socket, &m_fd_set_read );
         if( slave_socket > max_socket )
            max_socket = slave_socket;
      }

      int result_select = select( max_socket + 1, &m_fd_set_read, nullptr, nullptr, nullptr );
      if( -1 == result_select )
      {
         m_last_errno = errno;
         SYS_ERR( "select(%d) error: %d", m_master_socket, m_last_errno );
         continue;
      }
      SYS_MSG( "select(%d)", m_master_socket );

      process_slave_sockets( );

      if( FD_ISSET( m_master_socket, &m_fd_set_read ) )
      {
         int slave_socket = accept( m_master_socket, static_cast< sockaddr* >( nullptr ), nullptr );
         base::os::linux::set_nonblock( slave_socket );
         m_slave_sockets_set.insert( slave_socket );

         print_socket( slave_socket, "Host connected" );
      }
   }
}

void ConnectionProcessor::process_slave_sockets( )
{
   std::set< int > slave_sockets_to_remove_set;
   for( const auto& slave_socket : m_slave_sockets_set )
   {
      if( !FD_ISSET( slave_socket, &m_fd_set_read ) ) continue;
      if( eRead::DISCONNECTED == read_slave_socket( slave_socket ) )
      {
         slave_sockets_to_remove_set.insert( slave_socket );
         // connection closed
         ::shutdown( slave_socket, SHUT_RDWR );
         close( slave_socket );
      }
   }
   for( const auto& slave_socket : slave_sockets_to_remove_set )
      m_slave_sockets_set.erase( slave_socket );
}

ConnectionProcessor::eRead ConnectionProcessor::read_slave_socket( const int slave_socket )
{
   ssize_t read_size = recv( slave_socket, mp_buffer, base::configuration::dsi::buffer_size, 0 );
   if( 0 >= read_size )
   {
      m_last_errno = errno;
      if( m_last_errno != EAGAIN )
      {
         print_socket( slave_socket, "Host disconnected" );
         return eRead::DISCONNECTED;
      }

      SYS_ERR( "recv(%d): %ld bytes / error: %d", slave_socket, read_size, m_last_errno );
   }

   SYS_MSG( "recv(%d): %ld bytes", slave_socket, read_size );
   // DBG_INF( "recv(%d): %ld bytes", slave_socket, read_size );
   ssize_t send_size = send( slave_socket, mp_buffer, read_size, 0 );
   if( send_size != read_size )
   {
      m_last_errno = errno;
      SYS_ERR( "send(%d): %d", slave_socket, m_last_errno );
      return eRead::ERROR;
   }
   SYS_MSG( "send(%d): %zu bytes", slave_socket, send_size );

   return eRead::OK;
}
