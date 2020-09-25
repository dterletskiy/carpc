#include <string.h>
#include <errno.h>

#include "api/sys/oswrappers/linux/kernel.hpp"
#include "api/sys/oswrappers/linux/socket.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Socket"



namespace base::os::linux::socket {

   int error = 0;



   void configuration::print( const std::string& _message ) const
   {
      SYS_INF( "%s => domain: %d / type: %d / protocole: %d / address: %s / port: %d", _message.c_str( ), domain, type, protocole, address.c_str( ), port );
   }

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

      print( m_addr );
   }

   socket_addr::~socket_addr( )
   {
      free( m_addr );
   }

   const sockaddr* const socket_addr::addr( ) const
   {
      return m_addr;
   }

   const socklen_t socket_addr::len( ) const
   {
      return m_len;
   }



   fd::fd( )
   {
      mp_read = new fd_set;
      mp_write = new fd_set;
      mp_except = new fd_set;
   }

   fd::~fd( )
   {
      delete mp_read;
      delete mp_write;
      delete mp_except;
   }

   void fd::reset( )
   {
      FD_ZERO( mp_read );
      FD_ZERO( mp_write );
      FD_ZERO( mp_except );
   }

   void fd::set( const tSocket _socket, const eType type )
   {
      fd_set* p_fd_set = convert( type );
      FD_SET( _socket, p_fd_set );
   }

   void fd::set( const std::set< tSocket > socket_set, const std::set< eType > type_set )
   {
      std::set< fd_set* > p_fd_set = convert( type_set );
      for( const auto& _socket : socket_set )
         set( _socket, p_fd_set );
   }

   void fd::clear( const tSocket _socket, const eType type )
   {
      fd_set* p_fd_set = convert( type );
      FD_CLR( _socket, p_fd_set );
   }

   void fd::clear( const std::set< tSocket > socket_set, const std::set< eType > type_set )
   {
      std::set< fd_set* > p_fd_set = convert( type_set );
      for( const auto& _socket : socket_set )
         clear( _socket, p_fd_set );
   }

   bool fd::is_set( const tSocket _socket, const eType type )
   {
      fd_set* p_fd_set = convert( type );

      return p_fd_set ? FD_ISSET( _socket, p_fd_set ) : false;
   }

   void fd::set( tSocket _socket, std::set< fd_set* > p_fd_set )
   {
      for( const auto& item : p_fd_set )
         FD_SET( _socket, item );
   }

   void fd::clear( tSocket _socket, std::set< fd_set* > p_fd_set )
   {
      for( auto& item : p_fd_set )
         FD_CLR( _socket, item );
   }

   fd_set* fd::convert( const eType type ) const
   {
      switch( type )
      {
         case eType::READ:       return mp_read;
         case eType::WRITE:      return mp_write;
         case eType::EXCEPT:     return mp_except;
         default:                return nullptr;
      }
      return nullptr;
   }

   std::set< fd_set* > fd::convert( const std::set< eType > type_set ) const
   {
      std::set< fd_set* > fds;
      for( auto& type : type_set )
      {
         switch( type )
         {
            case eType::READ:       fds.insert( mp_read );     break;
            case eType::WRITE:      fds.insert( mp_write );    break;
            case eType::EXCEPT:     fds.insert( mp_except );   break;
            default:                                           break;
         }
      }
      return fds;
   }
   
   fd_set* const fd::read( ) const
   {
      return mp_read;
   }
   
   fd_set* const fd::write( ) const
   {
      return mp_write;
   }
   
   fd_set* const fd::except( ) const
   {
      return mp_except;
   }

} // namespace base::os::linux::socket



namespace base::os::linux::socket {

   void print( const sockaddr* sa )
   {
      char* c_address = nullptr;
      std::string domain( "Unknown AF" );

      switch( sa->sa_family )
      {
         case AF_UNIX:
         {
            domain = "AF_UNIX";
            const size_t maxlen = 256;
            c_address = (char*)malloc( maxlen );
            struct sockaddr_un* addr_un = (struct sockaddr_un*)sa;
            strncpy( c_address, addr_un->sun_path, maxlen );
            break;
         }

         case AF_INET:
         {
            domain = "AF_INET";
            const size_t maxlen = INET_ADDRSTRLEN;
            c_address = (char*)malloc( maxlen );
            struct sockaddr_in* addr_in = (struct sockaddr_in*)sa;
            inet_ntop( AF_INET, &(addr_in->sin_addr), c_address, maxlen );
            break;
         }

         case AF_INET6:
         {
            domain = "AF_INET6";
            const size_t maxlen = INET6_ADDRSTRLEN;
            c_address = (char*)malloc( maxlen );
            struct sockaddr_in6* addr_in6 = (struct sockaddr_in6*)sa;
            inet_ntop( AF_INET6, &(addr_in6->sin6_addr), c_address, maxlen );
            break;
         }

         default:
         {
            const size_t maxlen = 256;
            c_address = (char*)malloc( maxlen );
            strncpy( c_address, "Unknown AF", maxlen );
            break;
         }
      }

      SYS_INF( "sockaddr: %p / domain: %s / address: %s", sa, domain.c_str( ), c_address );
      free( c_address );
   }

   void info( const tSocket _socket, const char* _message )
   {
      struct sockaddr_in address;
      socklen_t addrlen = sizeof( address );

      // getpeername( _socket , (struct sockaddr*)&address , (socklen_t*)&addrlen );
      getsockname( _socket , (struct sockaddr*)&address , (socklen_t*)&addrlen );
      SYS_INF( "%s, ip:port/socket - %s:%d/%d ", _message, inet_ntoa(address.sin_addr), ntohs(address.sin_port), _socket );

      // printf( "%hd / %hu / %u / ", address.sin_family, address.sin_port, address.sin_addr.s_addr );
      // for( size_t i = 0; i < 8; ++i )
      //    printf( "%#x ", address.sin_zero[i] );
      // printf( "\n" );
   }

   void info( const tSocket _socket, configuration& _config )
   {
      struct sockaddr_in address;
      socklen_t addrlen = sizeof( address );

      // getpeername( _socket , (struct sockaddr*)&address , (socklen_t*)&addrlen );
      getsockname( _socket , (struct sockaddr*)&address , (socklen_t*)&addrlen );
      _config.address = inet_ntoa(address.sin_addr);
      _config.port = ntohs(address.sin_port);

   }

   const tSocket create_server( const configuration& _config )
   {
      tSocket _socket = base::os::linux::socket::socket( _config );
      if( InvalidSocket == _socket )
         return InvalidSocket;
      if( false == base::os::linux::socket::bind( _socket, _config ) )
         return InvalidSocket;
      base::os::linux::set_nonblock( _socket );
      if( false == base::os::linux::socket::listen( _socket ) )
         return InvalidSocket;
      return _socket;
   }

   const tSocket create_clint( const configuration& _config )
   {
      tSocket _socket = base::os::linux::socket::socket( _config );
      if( InvalidSocket == _socket )
         return InvalidSocket;
      if( false == base::os::linux::socket::connect( _socket, _config ) )
         return InvalidSocket;
      base::os::linux::set_nonblock( _socket );
      return _socket;
   }

   const tSocket socket( const int _domain, const int _type, const int _protocole )
   {
      tSocket _socket = ::socket( _domain, _type, _protocole );
      error = errno;
      if( -1 == _socket )
      {
         SYS_ERR( "socket(%d, %d, %d) error: %d", _domain, _type, _protocole, error );
         return _socket;
      }

      SYS_MSG( "socket(%d)", _socket );
      return _socket;
   }

   const tSocket socket( const configuration _config )
   {
      return socket( _config.domain, _config.type, _config.protocole );
   }

   const bool bind( const tSocket _socket, const sockaddr* _address, const socklen_t _address_len )
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

   const bool bind( const tSocket _socket, const int _domain, const char* const _address, const int _port )
   {
      socket_addr sa( _domain, _address, _port );
      return base::os::linux::socket::bind( _socket, sa.addr( ), sa.len( ) );
   }

   const bool bind( const tSocket _socket, const configuration _config )
   {
      return base::os::linux::socket::bind( _socket, _config.domain, _config.address.c_str( ), _config.port );
   }

   const bool connect( const tSocket _socket, const sockaddr* _address, const socklen_t _address_len )
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

   const bool connect( const tSocket _socket, const int _domain, const char* const _address, const int _port )
   {
      socket_addr sa( _domain, _address, _port );
      return base::os::linux::socket::connect( _socket, sa.addr( ), sa.len( ) );
   }

   const bool connect( const tSocket _socket, const configuration _config )
   {
      return base::os::linux::socket::connect( _socket, _config.domain, _config.address.c_str( ), _config.port );
   }

   const bool listen( const tSocket _socket, const int _backlog )
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

   const ssize_t send( const tSocket _socket, const void* _buffer, const size_t _size, const int _flags )
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

   const ssize_t recv( const tSocket _socket, void* const _buffer, const size_t _size, const int _flags )
   {
      if( nullptr == _buffer )
      {
         SYS_ERR( "recv(%d): buffer is nullptr", _socket );
         return 0;
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

   const tSocket accept( const tSocket _socket, sockaddr* const _address, socklen_t* const _address_len )
   {
      tSocket slave_socket = ::accept( _socket, _address, _address_len );
      error = errno;
      if( -1 == slave_socket )
      {
         SYS_ERR( "accept(%d): error %d", _socket, error );
         return slave_socket;
      }
      SYS_MSG( "accept(%d): %d slave socket", _socket, slave_socket );
      return slave_socket;
   }

   const bool select( const tSocket _max_socket, fd_set* const _fd_set_read, fd_set* const _fd_set_write, fd_set* const _fd_set_except, timeval* const _timeout )
   {
      int result = ::select( _max_socket + 1, _fd_set_read, _fd_set_write, _fd_set_except, _timeout );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "select(%d) error: %d", _max_socket + 1, error );
         return false;
      }
      SYS_MSG( "select(%d)", _max_socket + 1 );
      return true;
   }

   const bool select( const tSocket _max_socket, fd& _fd, timeval* const _timeout )
   {
      return base::os::linux::socket::select( _max_socket, _fd.read( ), _fd.write( ), _fd.except( ), _timeout );
   }

   void close( tSocket _socket )
   {
      SYS_MSG( "%d", _socket );
      ::shutdown( _socket, SHUT_RDWR );
      ::close( _socket );
   }

} // namespace base::os::linux::socket



namespace base::os::linux {

   bool get_mac( const std::string& connection_name, std::string& connection_mac )
   {
      socket::tSocket fd = socket::socket( AF_UNIX, SOCK_DGRAM, IPPROTO_IP );
      if( -1 == fd )
         return false;

      struct ifreq ifreq_data;
      strcpy( ifreq_data.ifr_name, connection_name.c_str( ) );
      int result = ioctl( fd, SIOCGIFHWADDR, &ifreq_data );
      int errno_ioctl = errno;
      SYS_INF( "ioctl result: %d", result );
      if( 0 != result )
      {
         SYS_ERR( "error: %d", errno_ioctl );
         return false;
      }

      connection_mac.clear( );
      for( size_t i = 0; i < 6; ++i )
      {
         char buffer[3];
         sprintf( buffer, "%02x", ( uint8_t )ifreq_data.ifr_addr.sa_data[i] );
         connection_mac += buffer;
      }
      SYS_INF( "Connection: %s / MAC: %s", connection_name.c_str( ), connection_mac.c_str( ) );

      return true;
   }

} // namespace base::os::linux

