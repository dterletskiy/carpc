#include "api/sys/oswrappers/linux/kernel.hpp"
#include "api/sys/oswrappers/Socket.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Socket"



using namespace base::os;



Socket::Socket( const Info& info, const size_t buffer_capacity )
   : m_info( info )
   , m_buffer_capacity( buffer_capacity )
{
   mp_buffer = malloc( m_buffer_capacity );
}

Socket::Socket( linux::socket::tSocket socket, const size_t buffer_capacity )
   : m_socket( socket )
   , m_buffer_capacity( buffer_capacity )
{
   mp_buffer = malloc( m_buffer_capacity );
}

Socket::~Socket( )
{
   close( );
   free( mp_buffer );
}

void Socket::close( )
{
   linux::socket::close( m_socket );
}

void Socket::info( const std::string& message ) const
{
   linux::socket::info( m_socket, message.c_str( ) );
}

bool Socket::create( )
{
   m_socket = linux::socket::socket( m_info.socket_family, m_info.socket_type, m_info.socket_protocole );
   return -1 != m_socket;
}

bool Socket::bind( )
{
   return linux::socket::bind( m_socket, m_info.socket_family, m_info.server_address, m_info.server_port );
}

bool Socket::listen( )
{
   return linux::socket::listen( m_socket );
}

Socket Socket::accept( )
{
   linux::socket::tSocket socket = linux::socket::accept( m_socket );
   return Socket( socket, m_buffer_capacity );
}

bool Socket::connect( )
{
   return linux::socket::connect( m_socket, m_info.socket_family, m_info.server_address, m_info.server_port );
}

bool Socket::send( const void* p_buffer, const size_t size, const int flags )
{
   ssize_t send_size = linux::socket::send( m_socket, p_buffer, size, flags );
   return send_size == static_cast< ssize_t >( size );
}

bool Socket::recv( const int flags )
{
   fill_buffer( );
   m_buffer_size = linux::socket::recv( m_socket, mp_buffer, m_buffer_capacity, flags );
   return 0 < m_buffer_size;
}

bool Socket::select( linux::socket::tSocket max_socket )
{
   return linux::socket::select( max_socket + 1, m_fd );
}

linux::socket::tSocket Socket::socket( ) const
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



Server::Server( const Info& info, const size_t buffer_capacity )
   : Socket( info, buffer_capacity )
{
   unlink( info.server_address );
}

Server::~Server( )
{
}

void Server::fd_reset( )
{
   m_fd.reset( );
}

void Server::fd_init( )
{
   // m_fd.set( m_socket, os::linux::socket::fd::eType::READ );
   // m_max_socket = m_socket;
   // for( const auto& slave_socket : m_slave_sockets_set )
   // {
   //    m_fd.set( slave_socket, os::linux::socket::fd::eType::READ );
   //    if( slave_socket > m_max_socket )
   //       m_max_socket = slave_socket;
   // }
}
