#include "api/sys/tools/Tools.hpp"
#include "api/sys/oswrappers/Socket.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Socket"



using namespace carpc::os;



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

Socket::Socket( const os_linux::socket::configuration& configuration, const size_t buffer_capacity )
   : m_configuration( configuration )
   , m_buffer_capacity( buffer_capacity )
{
   mp_buffer = malloc( m_buffer_capacity );
}

Socket::Socket( const os_linux::socket::tSocket socket, const size_t buffer_capacity )
   : m_socket( socket )
   , m_buffer_capacity( buffer_capacity )
{
   mp_buffer = malloc( m_buffer_capacity );

   os_linux::socket::info( m_socket, m_configuration );
}

Socket::Socket( Socket&& other )
   : m_socket( other.m_socket )
   , m_id( other.m_id )
   , m_configuration( other.m_configuration )
   , mp_buffer( other.mp_buffer )
   , m_buffer_capacity( other.m_buffer_capacity )
   , m_buffer_size( other.m_buffer_size )
   , m_total_recv_size( other.m_total_recv_size )
   , m_total_send_size( other.m_total_send_size )
{
   other.m_socket = os_linux::socket::InvalidSocket;
   other.mp_buffer = nullptr;
}

Socket::~Socket( )
{
   close( );
   free( mp_buffer );
   SYS_INF( "total recv size: %zu", m_total_recv_size );
   SYS_INF( "total send size: %zu", m_total_send_size );
}

Socket::tSptr Socket::create_shared( const os_linux::socket::configuration& configuration, const size_t buffer_capacity )
{
   struct make_shared_enabler : public Socket
   {
      make_shared_enabler( const os_linux::socket::configuration& configuration, const size_t buffer_capacity )
         : Socket( configuration, buffer_capacity )
      { }
   };

   return std::make_shared< make_shared_enabler >( configuration, buffer_capacity );
}

Socket::tSptr Socket::create_shared( const os_linux::socket::tSocket socket, const size_t buffer_capacity )
{
   struct make_shared_enabler : public Socket
   {
      make_shared_enabler( const os_linux::socket::tSocket socket, const size_t buffer_capacity )
         : Socket( socket, buffer_capacity )
      { }
   };

   return std::make_shared< make_shared_enabler >( socket, buffer_capacity );
}

const bool Socket::operator<( const Socket& other ) const
{
   return m_id < other.m_id;
}

Socket& Socket::operator=( Socket&& other )
{
   if( this == &other )
      return *this;

   m_socket = other.m_socket;
   m_id = other.m_id;
   m_configuration = other.m_configuration;
   close( );
   free( mp_buffer );
   mp_buffer = other.mp_buffer;
   m_buffer_capacity = other.m_buffer_capacity;
   m_buffer_size = other.m_buffer_size;
   m_total_recv_size = other.m_total_recv_size;
   m_total_send_size = other.m_total_send_size;

   other.m_socket = os_linux::socket::InvalidSocket;
   other.m_id = ID::invalid;
   other.mp_buffer = nullptr;

   return *this;
}

void Socket::close( )
{
   os_linux::socket::close( m_socket );
}

void Socket::info( const std::string& message ) const
{
   os_linux::socket::info( m_socket, message.c_str( ) );
}

Socket::eResult Socket::create( )
{
   m_socket = os_linux::socket::socket( m_configuration );
   return os_linux::socket::InvalidSocket != m_socket ? eResult::OK : eResult::ERROR;
}

Socket::eResult Socket::bind( )
{
   unlink( m_configuration.address.c_str( ) );
   return os_linux::socket::bind( m_socket, m_configuration ) ? eResult::OK : eResult::ERROR;
}

Socket::eResult Socket::listen( )
{
   return os_linux::socket::listen( m_socket ) ? eResult::OK : eResult::ERROR;
}

Socket::tSptr Socket::accept( )
{
   sockaddr sa;
   socklen_t len = sizeof( sa );

   os_linux::socket::tSocket socket = os_linux::socket::accept( m_socket, &sa, &len );
   if( os_linux::socket::InvalidSocket == socket )
      return nullptr;

   os_linux::socket::print( &sa );

   return Socket::create_shared( socket, m_buffer_capacity );
}

Socket::eResult Socket::connect( )
{
   return os_linux::socket::connect( m_socket, m_configuration ) ? eResult::OK : eResult::ERROR;
}

Socket::eResult Socket::send( const void* p_buffer, const size_t size, const int flags )
{
   // MSG_NOSIGNAL flag is needed to avoid receiving SIGPIPE signal in case of sending to not connected socket.
   // https://stackoverflow.com/questions/19172804/crash-when-sending-data-without-connection-via-socket-in-linux
   ssize_t send_size = os_linux::socket::send( m_socket, p_buffer, size, flags | MSG_NOSIGNAL );
   m_total_send_size += send_size;
   return send_size == static_cast< ssize_t >( size ) ? eResult::OK : eResult::ERROR;
}

Socket::eResult Socket::recv( const int flags )
{
   fill_buffer( );
   ssize_t recv_size = os_linux::socket::recv( m_socket, mp_buffer, m_buffer_capacity, flags );
   if( 0 >= recv_size )
   {
      m_buffer_size = 0;
      return os_linux::socket::error != EAGAIN ? eResult::DISCONNECTED : eResult::ERROR;
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

carpc::RawBuffer Socket::buffer( ) const
{
   return { mp_buffer, m_buffer_size };
}

void Socket::fill_buffer( const char symbol )
{
   memset( mp_buffer, symbol, m_buffer_capacity );
}



SocketClient::SocketClient( const os_linux::socket::configuration& configuration, const size_t buffer_capacity )
   : Socket( configuration, buffer_capacity )
{
}

SocketClient::~SocketClient( )
{
}

SocketClient::tSptr SocketClient::create_shared( const os_linux::socket::configuration& configuration, const size_t buffer_capacity )
{
   struct make_shared_enabler : public SocketClient
   {
      make_shared_enabler( const os_linux::socket::configuration& configuration, const size_t buffer_capacity )
         : SocketClient( configuration, buffer_capacity )
      { }
   };

   return std::make_shared< make_shared_enabler >( configuration, buffer_capacity );
}



SocketServer::SocketServer( const os_linux::socket::configuration& configuration, const size_t buffer_capacity, IConsumer& consumer )
   : Socket( configuration, buffer_capacity )
   , m_consumer( consumer )
{
   // unlink( configuration.address );
}

SocketServer::~SocketServer( )
{
}

SocketServer::tSptr SocketServer::create_shared(
            const os_linux::socket::configuration& configuration, const size_t buffer_capacity, IConsumer& consumer
         )
{
   struct make_shared_enabler : public SocketServer
   {
      make_shared_enabler( const os_linux::socket::configuration& configuration, const size_t buffer_capacity, IConsumer& consumer )
         : SocketServer( configuration, buffer_capacity, consumer )
      { }
   };

   return std::make_shared< make_shared_enabler >( configuration, buffer_capacity, consumer );
}

void SocketServer::fd_reset( )
{
   m_fd.reset( );
}

void SocketServer::fd_init( )
{
   m_fd.set( socket( ), os_linux::socket::fd::eType::READ );
   m_max_socket = socket( );
   for( const auto& p_slave_socket : m_slave_sockets )
   {
      m_fd.set( p_slave_socket->socket( ), os_linux::socket::fd::eType::READ );
      if( p_slave_socket->socket( ) > m_max_socket )
         m_max_socket = p_slave_socket->socket( );
   }
}

bool SocketServer::select( )
{
   fd_reset( );
   fd_init( );
   if( false == os_linux::socket::select( m_max_socket, m_fd ) )
      return false;

   for( auto iterator = m_slave_sockets.begin( ); iterator != m_slave_sockets.end( ); ++iterator )
   {
      if( false == m_fd.is_set( (*iterator)->socket( ), os_linux::socket::fd::eType::READ ) )
         continue;

      const eResult result = (*iterator)->recv( );
      if( eResult::DISCONNECTED == result )
      {
         (*iterator)->info( "Host disconnected" );
         m_consumer.disconnected( *iterator );
         iterator = m_slave_sockets.erase( iterator );
         if( m_slave_sockets.end( ) == iterator )
            break;
      }
      else if( eResult::OK == result )
      {
         m_consumer.read_slave( *iterator );
      }
   }

   if( true == m_fd.is_set( socket( ), os_linux::socket::fd::eType::READ ) )
   {
      if( auto p_socket = accept( ) )
      {
         m_slave_sockets.push_back( p_socket );
         p_socket->info( "Host connected" );
         p_socket->unblock( );
         m_consumer.connected( p_socket );
      }
   }

   return true;
}

void SocketServer::calc_max( )
{
   m_max_socket = socket( );
   for( const auto& p_socket : m_slave_sockets )
      if( p_socket->socket( ) > m_max_socket )
         m_max_socket = p_socket->socket( );
}
