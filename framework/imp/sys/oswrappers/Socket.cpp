#include "api/sys/oswrappers/linux/kernel.hpp"
#include "api/sys/oswrappers/Socket.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Socket"



namespace {

   class socket_addr
   {
      public:
         socket_addr( const int _domain, const char* const _address, const int _port );
         socket_addr( const socket_addr& ) = delete;
         ~socket_addr( );
         socket_addr& operator=( const socket_addr& ) = delete;

      public:
         const sockaddr* const addr( ) const;
         const socklen_t len( ) const;
      private:
         sockaddr* m_addr = nullptr;
         socklen_t m_len = 0;
   };

   socket_addr::socket_addr( const int _domain, const char* const _address, const int _port )
   {
      switch( _domain )
      {
         case AF_UNIX:
         {
            SYS_INF( "AF_UNIX" );

            struct sockaddr_un* serv_addr_un = (sockaddr_un*)malloc( sizeof( sockaddr_un ) );
            memset( serv_addr_un, 0, sizeof( sockaddr_un ) );

            serv_addr_un->sun_family = _domain;
            strncpy( serv_addr_un->sun_path, _address, sizeof( serv_addr_un->sun_path ) - 1 );

            m_addr = reinterpret_cast< sockaddr* >( serv_addr_un );
            m_len = sizeof( serv_addr_un->sun_family ) + strlen( serv_addr_un->sun_path );
            break;
         }
         case AF_INET:
         {
            SYS_INF( "AF_INET" );

            struct sockaddr_in* serv_addr_in = (sockaddr_in*)malloc( sizeof( sockaddr_in ) );
            memset( serv_addr_in, 0, sizeof( sockaddr_in ) );

            serv_addr_in->sin_family = _domain;
            serv_addr_in->sin_addr.s_addr = inet_addr( _address );
            serv_addr_in->sin_port = htons( _port );

            m_addr = reinterpret_cast< sockaddr* >( serv_addr_in );
            m_len = sizeof( sockaddr_in );
            break;
         }
         case AF_INET6:
         default:          break;
      }
   }

   socket_addr::~socket_addr( )
   {
      // free( m_addr );
   }

   const sockaddr* const socket_addr::addr( ) const
   {
      return m_addr;
   }

   const socklen_t socket_addr::len( ) const
   {
      return m_len;
   }

} // namespace base



namespace base::socket {

   int error = 0;

   void info( const int _socket, const char* _message )
   {
      struct sockaddr_in address;
      socklen_t addrlen = sizeof( address );

      getpeername( _socket , (struct sockaddr*)&address , (socklen_t*)&addrlen );
      SYS_INF( "%s, ip:port/socket - %s:%d/%d ", _message, inet_ntoa(address.sin_addr), ntohs(address.sin_port), _socket );

      // printf( "%hd / %hu / %u / ", address.sin_family, address.sin_port, address.sin_addr.s_addr );
      // for( size_t i = 0; i < 8; ++i )
      //    printf( "%#x ", address.sin_zero[i] );
      // printf( "\n" );
   }

   const int socket( const int _domain, const int _type, const int _protocole )
   {
      int _socket = ::socket( _domain, _type, _protocole );
      error = errno;
      if( -1 == _socket )
      {
         SYS_ERR( "socket(%d, %d, %d) error: %d", _domain, _type, _protocole, error );
         return _socket;
      }

      SYS_MSG( "socket(%d)", _socket );
      return _socket;
   }

   const bool bind( const int _socket, const sockaddr* _address, const socklen_t _address_len )
   {
      if( nullptr == _address )
      {
         SYS_ERR( "bind(%d): address is nullptr", _socket );
         return false;
      }

      int result = ::bind( _socket, _address, _address_len );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "bind(%d) error: %d", _socket, error );
         return false;
      }
      SYS_MSG( "bind(%d)", _socket );
      return true;
   }

   const bool bind( const int _socket, const int _domain, const char* const _address, const int _port )
   {
      socket_addr sa( _domain, _address, _port );
      return base::socket::bind( _socket, sa.addr( ), sa.len( ) );
   }

   const bool connect( const int _socket, const sockaddr* _address, const socklen_t _address_len )
   {
      if( nullptr == _address )
      {
         SYS_ERR( "connect(%d): address is nullptr", _socket );
         return false;
      }

      int result = ::connect( _socket, _address, _address_len );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "connect(%d): error %d", _socket, error );
         return false;
      }
      SYS_MSG( "connect(%d)", _socket );
      return true;
   }

   const bool connect( const int _socket, const int _domain, const char* const _address, const int _port )
   {
      socket_addr sa( _domain, _address, _port );
      return base::socket::connect( _socket, sa.addr( ), sa.len( ) );
   }

   const bool listen( const int _socket, const int _backlog )
   {
      int result = ::listen( _socket, _backlog );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "listen(%d) error: %d", _socket, error );
         return false;
      }
      SYS_MSG( "listen(%d)", _socket );
      return true;
   }

   const ssize_t send( const int _socket, const void* _buffer, const size_t _size, const int _flags )
   {
      if( nullptr == _buffer )
      {
         SYS_ERR( "send(%d): buffer is nullptr", _socket );
         return 0;
      }

      ssize_t size = ::send( _socket, _buffer, _size, _flags );
      error = errno;
      if( size != static_cast< ssize_t >( _size ) )
      {
         SYS_ERR( "send(%d): error %d / bytes %zd", _socket, error, size );
         return size;
      }
      SYS_MSG( "send(%d): %zd bytes", _socket, size );
      return size;
   }

   const ssize_t recv( const int _socket, void* const _buffer, const size_t _size, const int _flags )
   {
      if( nullptr == _buffer )
      {
         SYS_ERR( "recv(%d): buffer is nullptr", _socket );
         return false;
      }

      ssize_t size = ::recv( _socket, _buffer, _size, _flags );
      error = errno;
      if( 0 >= size )
      {
         SYS_ERR( "recv(%d): error %d", _socket, error );
         return size;
      }
      SYS_MSG( "recv(%d): %zd bytes", _socket, size );
      return size;
   }

   const int accept( const int _socket, sockaddr* const _address, socklen_t* const _address_len )
   {
      int slave_socket = ::accept( _socket, _address, _address_len );
      error = errno;
      if( -1 == slave_socket )
      {
         SYS_ERR( "accept(%d): error %d", _socket, error );
         return slave_socket;
      }
      SYS_MSG( "accept(%d): %d slave socket", _socket, slave_socket );
      return slave_socket;
   }

   const bool select( const int _max_socket, fd_set* const _fd_set_read, fd_set* const _fd_set_write, fd_set* const _fd_set_except, timeval* const _timeout )
   {
      int result = ::select( _max_socket + 1, _fd_set_read, _fd_set_write, _fd_set_except, _timeout );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "select(%d) error: %d", _max_socket, error );
         return false;
      }
      SYS_MSG( "select(%d)", _max_socket );
      return true;
   }

   void close( int _socket )
   {
      SYS_MSG( "%d", _socket );
      ::shutdown( _socket, SHUT_RDWR );
      ::close( _socket );
   }

} // namespace base::socket







using namespace base;



Socket::Socket( const int socket_family, const int socket_type, const int socket_protocole, const char* server_address, const int server_port, const size_t buffer_capacity )
   : m_socket_family( socket_family )
   , m_socket_type( socket_type )
   , m_socket_protocole( socket_protocole )
   , m_server_address( server_address )
   , m_server_port( server_port )
   , m_buffer_capacity( buffer_capacity )
{
   mp_buffer = malloc( m_buffer_capacity );
}

Socket::~Socket( )
{
   close( );
   free( mp_buffer );
   free( mp_sockaddr );
}

void Socket::init( )
{
   switch( m_socket_family )
   {
      case AF_UNIX:  init_un( ); break;
      case AF_INET:  init_in( ); break;
      case AF_INET6:             break;
      default:                   break;
   }
}

void Socket::close( )
{
   ::shutdown( m_socket, SHUT_RDWR );
   ::close( m_socket );
}

void Socket::init_un( )
{
   SYS_INF( "AF_UNIX" );

   struct sockaddr_un* serv_addr_un = (sockaddr_un*)malloc( sizeof( sockaddr_un ) );
   memset( serv_addr_un, 0, sizeof( sockaddr_un ) );

   serv_addr_un->sun_family = m_socket_family;
   strncpy( serv_addr_un->sun_path, m_server_address, sizeof( serv_addr_un->sun_path ) - 1 );

   m_sockaddr_size = sizeof( serv_addr_un->sun_family ) + strlen( serv_addr_un->sun_path );
   mp_sockaddr = reinterpret_cast< sockaddr* >( serv_addr_un );
}

void Socket::init_in( )
{
   SYS_INF( "AF_INET" );

   struct sockaddr_in* serv_addr_in = (sockaddr_in*)malloc( sizeof( sockaddr_in ) );
   memset( serv_addr_in, 0, sizeof( sockaddr_in ) );

   serv_addr_in->sin_family = m_socket_family;
   serv_addr_in->sin_addr.s_addr = inet_addr( m_server_address );
   // serv_addr_in->sin_addr.s_addr = htonl( INADDR_ANY );
   // serv_addr_in->sin_addr.s_addr = htonl( INADDR_LOOPBACK );
   serv_addr_in->sin_port = htons( m_server_port );

   m_sockaddr_size = sizeof( sockaddr_in );
   mp_sockaddr = reinterpret_cast< sockaddr* >( serv_addr_in );
}

void Socket::print( const std::string& message ) const
{
   struct sockaddr_in address;
   socklen_t addrlen = sizeof( address );
   getpeername( m_socket , (struct sockaddr*)&address , (socklen_t*)&addrlen );
   SYS_INF( "%s, ip:port/socket - %s:%d/%d ", message.c_str( ), inet_ntoa(address.sin_addr), ntohs(address.sin_port), m_socket );

   printf( "%hd / %hu / %u / ", address.sin_family, address.sin_port, address.sin_addr.s_addr );
   for( size_t i = 0; i < 8; ++i )
      printf( "%#x ", address.sin_zero[i] );
   printf( "\n" );
}

const bool Socket::create( )
{
   m_socket = ::socket( m_socket_family, m_socket_type, m_socket_protocole );
   if( -1 == m_socket )
   {
      m_last_errno = errno;
      SYS_ERR( "socket(%d, %d, %d) error: %d", m_socket_family, m_socket_type, m_socket_protocole, m_last_errno );
      return false;
   }

   SYS_MSG( "socket(%d)", m_socket );
   return true;
}

const bool Socket::bind( )
{
   int result = ::bind( m_socket, mp_sockaddr, m_sockaddr_size );
   if( -1 == result )
   {
      m_last_errno = errno;
      SYS_ERR( "bind(%d) error: %d", m_socket, m_last_errno );
      return false;
   }
   SYS_MSG( "bind(%d)", m_socket );
   return true;
}

const bool Socket::connect( )
{
   int result = ::connect( m_socket, mp_sockaddr, m_sockaddr_size );
   if( -1 == result )
   {
      m_last_errno = errno;
      SYS_ERR( "connect(%d): error: %d", m_socket, m_last_errno );
      return false;
   }
   SYS_MSG( "connect(%d)", m_socket );
   return true;
}

const bool Socket::listen( )
{
   int result = ::listen( m_socket, SOMAXCONN );
   if( -1 == result )
   {
      m_last_errno = errno;
      SYS_ERR( "listen(%d) error: %d", m_socket, m_last_errno );
      return false;
   }
   SYS_MSG( "listen(%d)", m_socket );
   return true;
}

const bool Socket::send( const void* p_buffer, const size_t size, const int flags )
{
   ssize_t send_size = ::send( m_socket, p_buffer, size, flags );
   if( send_size != static_cast< ssize_t >( size ) )
   {
      m_last_errno = errno;
      SYS_ERR( "send(%d): error %d / %zd bytes", m_socket, m_last_errno, send_size );
      return false;
   }
   SYS_MSG( "send(%d): %zd bytes", m_socket, send_size );
   return true;
}

const bool Socket::recv( const int flags )
{
   fill_buffer( );

   m_buffer_size = ::recv( m_socket, mp_buffer, m_buffer_capacity, flags );
   if( 0 >= m_buffer_size )
   {
      m_last_errno = errno;
      SYS_ERR( "recv(%d): error: %d", m_socket, m_last_errno );
      m_buffer_size = 0;
      return false;
   }
   SYS_MSG( "recv(%d): %zu bytes", m_socket, m_buffer_size );
   return true;
}

const int Socket::accept( )
{
   int socket = ::accept( m_socket, static_cast< sockaddr* >( nullptr ), nullptr );
   return socket;
}

const bool Socket::select( const int max_socket, fd_set* const fd_set_read, fd_set* const fd_set_write, fd_set* const fd_set_except, timeval* const timeout )
{
   int result = ::select( max_socket + 1, fd_set_read, fd_set_write, fd_set_except, timeout );
   if( -1 == result )
   {
      m_last_errno = errno;
      SYS_ERR( "select(%d) error: %d", m_socket, m_last_errno );
      return false;
   }
   SYS_MSG( "select(%d)", m_socket );
   return true;
}

const int Socket::socket( ) const
{
   return m_socket;
}

const void* const Socket::buffer( size_t& size ) const
{
   size = m_buffer_size;
   return mp_buffer;
}

void Socket::fill_buffer( const char symbol )
{
   memset( mp_buffer, symbol, m_buffer_capacity );
}



Server::Server( const int socket_family, const int socket_type, const int socket_protocole, const char* server_address, const int server_port, const size_t buffer_capacity )
   : Socket( socket_family, socket_type, socket_protocole, server_address, server_port, buffer_capacity )
{
   unlink( server_address );
   init( );

   mp_fd_set_read = new fd_set;
   mp_fd_set_write = new fd_set;
   mp_fd_set_except = new fd_set;
}

Server::~Server( )
{
}

void Server::fd_set_reset( )
{
   FD_ZERO( mp_fd_set_read );
   FD_ZERO( mp_fd_set_write );
   FD_ZERO( mp_fd_set_except );
}

void Server::fd_set_init( )
{
   FD_SET( m_socket, mp_fd_set_read );
   int m_max_socket = m_socket;
   for( const auto& slave_socket : m_slave_sockets_set )
   {
      FD_SET( slave_socket, mp_fd_set_read );
      if( slave_socket > m_max_socket )
         m_max_socket = slave_socket;
   }
}

const bool Server::select( )
{
   return Socket::select( m_max_socket, mp_fd_set_read, mp_fd_set_write, mp_fd_set_except, nullptr );
}
