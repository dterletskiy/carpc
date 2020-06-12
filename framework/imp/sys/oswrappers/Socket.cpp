#include "api/sys/oswrappers/Socket.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Socket"



using namespace base::os;



const char* Socket::c_str( const eResult comm_type )
{
   switch( comm_type )
   {
      case eResult::OK:             return "eResult::OK";
      case eResult::ERROR:          return "eResult::ERROR";
      case eResult::DISCONNECTED:   return "eResult::DISCONNECTED";
      default:                      return "eResult::ERROR";
   }
}

Socket::Socket( const linux::socket::configuration& configuration, const size_t buffer_capacity )
   : m_configuration( configuration )
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
   SYS_INF( "total recv size: %zu", m_total_recv_size );
   SYS_INF( "total send size: %zu", m_total_send_size );
}

const bool Socket::operator<( const Socket& socket ) const
{
   return m_socket < socket.m_socket;
}

void Socket::close( )
{
   linux::socket::close( m_socket );
}

void Socket::info( const std::string& message ) const
{
   linux::socket::info( m_socket, message.c_str( ) );
}

Socket::eResult Socket::create( )
{
   m_socket = linux::socket::socket( m_configuration );
   return linux::socket::InvalidSocket != m_socket ? eResult::OK : eResult::ERROR;
}

Socket::eResult Socket::bind( )
{
   unlink( m_configuration.address );
   return linux::socket::bind( m_socket, m_configuration ) ? eResult::OK : eResult::ERROR;
}

Socket::eResult Socket::listen( )
{
   return linux::socket::listen( m_socket ) ? eResult::OK : eResult::ERROR;
}

Socket::tSptr Socket::accept( )
{
   linux::socket::tSocket socket = linux::socket::accept( m_socket );
   if( linux::socket::InvalidSocket == socket )
      return nullptr;
   return tSptr( new Socket( socket, m_buffer_capacity ) );
}

Socket::eResult Socket::connect( )
{
   return linux::socket::connect( m_socket, m_configuration ) ? eResult::OK : eResult::ERROR;
}

Socket::eResult Socket::send( const void* p_buffer, const size_t size, const int flags )
{
   ssize_t send_size = linux::socket::send( m_socket, p_buffer, size, flags );
   m_total_send_size += send_size;
   return send_size == static_cast< ssize_t >( size ) ? eResult::OK : eResult::ERROR;
}

Socket::eResult Socket::recv( const int flags )
{
   fill_buffer( );
   ssize_t recv_size = linux::socket::recv( m_socket, mp_buffer, m_buffer_capacity, flags );
   if( 0 >= recv_size )
   {
      m_buffer_size = 0;
      return linux::socket::error != EAGAIN ? eResult::DISCONNECTED : eResult::ERROR;
   }

   m_total_recv_size += recv_size;
   m_buffer_size = static_cast< size_t >( recv_size );
   return eResult::OK;
}

Socket::eResult Socket::recv( tRecvCallback callback, const int flags )
{
   eResult result = recv( flags );
   callback( mp_buffer, m_buffer_size, result );
   return result;
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



SocketClient::SocketClient( const linux::socket::configuration& configuration, const size_t buffer_capacity )
   : Socket( configuration, buffer_capacity )
{
}

SocketClient::~SocketClient( )
{
}



SocketServer::SocketServer( const linux::socket::configuration& configuration, const size_t buffer_capacity )
   : Socket( configuration, buffer_capacity )
{
   // unlink( configuration.address );
}

SocketServer::~SocketServer( )
{
}

void SocketServer::fd_reset( )
{
   m_fd.reset( );
}

void SocketServer::fd_init( )
{
   m_fd.set( socket( ), linux::socket::fd::eType::READ );
   m_max_socket = socket( );
   for( const auto& p_slave_socket : m_slave_sockets )
   {
      m_fd.set( p_slave_socket->socket( ), linux::socket::fd::eType::READ );
      if( p_slave_socket->socket( ) > m_max_socket )
         m_max_socket = p_slave_socket->socket( );
   }
}

bool SocketServer::select( )
{
   fd_reset( );
   fd_init( );
   if( false == linux::socket::select( m_max_socket, m_fd ) )
      return false;

   for( auto iterator = m_slave_sockets.begin( ); iterator != m_slave_sockets.end( ); ++iterator )
   {
      if( false == m_fd.is_set( (*iterator)->socket( ), linux::socket::fd::eType::READ ) )
         continue;

      const eResult result = (*iterator)->recv( );
      if( eResult::DISCONNECTED == result )
      {
         (*iterator)->info( "Host disconnected" );
         iterator = m_slave_sockets.erase( iterator );
         if( m_slave_sockets.end( ) == iterator )
            break;
      }
      else if( eResult::OK == result )
      {
         read_slave( *iterator );
      }
   }

   if( true == m_fd.is_set( socket( ), linux::socket::fd::eType::READ ) )
   {
      if( auto p_socket = accept( ) )
      {
         m_slave_sockets.push_back( p_socket );
         p_socket->info( "Host connected" );
         p_socket->unblock( );
      }
   }

   return true;
}

void SocketServer::read_slave( tSptr socket )
{

}

void SocketServer::calc_max( )
{
   m_max_socket = socket( );
   for( const auto p_socket : m_slave_sockets )
      if( p_socket->socket( ) > m_max_socket )
         m_max_socket = p_socket->socket( );
}
