#include "configuration.hpp"
#include "common/Types.hpp"
#include "common/ByteBuffer.hpp"

#include "Trace.hpp"
#define CLASS_ABBR "MAIN"



int set_nonblocke( int fd )
{
   int flags = -1;
#if defined( O_NONBLOCK )
   if( -1 == ( flags = fcntl( fd, F_GETFL, 0 ) ) )
      flags = 0;
   return fcntl( fd, F_SETFL, flags | O_NONBLOCK );
#else
   flags = 1;
      return ioctl( fd, FIOBIO, &flags );
#endif
}

void* thread_loop( void* param )
{
   int client_socket = *( static_cast< int* >( param ) );
   struct sockaddr_in address;
   socklen_t addrlen = sizeof( address );
   getpeername( client_socket , (struct sockaddr*)&address , (socklen_t*)&addrlen );
   DBG_MSG( "Host connected, ip:port/socket - %s:%d/%d", inet_ntoa(address.sin_addr), ntohs(address.sin_port), client_socket );

   const size_t buffer_size = 2048;
   char buffer[ buffer_size ];
   int error = 0;
   ssize_t read_size = 1;
   ssize_t send_size = 0;

   while( 0 < read_size )
   {
      memset( buffer, 0, sizeof(buffer) );

      read_size = recv( client_socket, buffer, buffer_size, 0 );
      error = errno;
      if( 0 >= read_size )
      {
         DBG_ERR( "recv(%d): %d", client_socket, error );
         continue;
      }
      DBG_MSG( "recv(%d)", client_socket );

      DBG_MSG( "%s:%d/%d sent message", inet_ntoa(address.sin_addr), ntohs(address.sin_port), client_socket );
      base::ByteBuffer byte_buffer( (void*)buffer, read_size );
      byte_buffer.dump( );

      send_size = send( client_socket, byte_buffer.buffer( ), byte_buffer.size( ), 0 );
      error = errno;
      if( byte_buffer.size( ) != send_size )
      {
         DBG_ERR( "send(%d): %d", client_socket, error );
         continue;
      }
      DBG_MSG( "send(%d)", client_socket );
   }

   DBG_MSG( "Host disconnected, ip:port/socket - %s:%d/%d ", inet_ntoa(address.sin_addr), ntohs(address.sin_port), client_socket );
   close( client_socket );

   return nullptr;
}

int main( int argc, char *argv[] )
{
   int master_socket = socket( socket_family, socket_type, socket_protocole );

   int result_bind = -1;
   if( AF_UNIX == socket_family )
   {
      struct sockaddr_un serv_addr_un;
      memset( &serv_addr_un, 0, sizeof(serv_addr_un) );
      serv_addr_un.sun_family = socket_family;
      strncpy( serv_addr_un.sun_path, server_address, sizeof(serv_addr_un.sun_path) - 1 );
      unlink( server_address );
      int serv_addr_un_size = sizeof(serv_addr_un.sun_family) + strlen(serv_addr_un.sun_path);

      result_bind = bind( master_socket, (sockaddr*)(&serv_addr_un), serv_addr_un_size );
   }
   else if( AF_INET == socket_family )
   {
      struct sockaddr_in serv_addr_in;
      memset( &serv_addr_in, 0, sizeof(serv_addr_in) );
      serv_addr_in.sin_family = socket_family;
      // serv_addr_in.sin_addr.s_addr = inet_addr(inet_address);
      serv_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
      serv_addr_in.sin_port = htons(server_port);
      int serv_addr_in_size = sizeof(serv_addr_in);

      result_bind = bind( master_socket, (sockaddr*)(&serv_addr_in), serv_addr_in_size );
   }

   if( 0 > result_bind )
   {
      DBG_ERR( "bind(%d) error: %d", master_socket, errno );
      return errno;
   }
   DBG_MSG( "bind(%d)", master_socket );

   if( 0 > listen( master_socket, 10 ) )
   {
      DBG_ERR( "listen(%d) error: %d", master_socket, errno );
      return errno;
   }
   DBG_MSG( "listen(%d)", master_socket );

   const size_t buffer_size = 2048;
   char buffer[ buffer_size ];
   while( true )
   {
      DBG_MSG( "Waiting connection..." );
      memset( buffer, 0, sizeof(buffer) );
      int client_socket = accept( master_socket, static_cast< sockaddr* >( nullptr ), nullptr );
      if( 0 > client_socket )
      {
         DBG_ERR( "accept(%d): %d", master_socket, errno );
      }
      DBG_MSG( "accept(%d)", client_socket );

      // time_t ticks = time( nullptr );
      // snprintf( buffer, sizeof(buffer), "%.24s\r\n", ctime( &ticks ) );
      // if( send( client_socket, buffer, strlen( buffer ), 0 ) != strlen( buffer ) )
      // {
      //    DBG_ERR( "send(%d): %d", client_socket, errno );
      // }
      // else
      // {
      //    DBG_MSG( "send(%d)", client_socket );
      // }

      pthread_t thread_id;
      if( 0 != pthread_create( &thread_id, nullptr, thread_loop, &client_socket ) )
      {
         DBG_ERR( "Failed to create thread: %d", errno );
      }

      sleep( 1 );
   }
}
