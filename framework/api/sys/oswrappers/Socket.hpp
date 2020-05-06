#pragma once

#include <sys/un.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string>



namespace base::socket {

   extern int error;

   void info( const int _socket, const char* _message = "socket" );

   const int socket(
                          const int _domain = AF_UNIX
                        , const int _type = SOCK_STREAM
                        , const int _protocole = 0
   );

   const bool bind(
                          const int _socket
                        , const sockaddr* _address
                        , const socklen_t _address_len
   );

   const bool bind(
                          const int _socket
                        , const int _domain
                        , const char* const _address
                        , const int _port
   );

   const bool connect(
                          const int _socket
                        , const sockaddr* _address
                        , const socklen_t _address_len
   );

   const bool connect(
                          const int _socket
                        , const int _domain
                        , const char* const _address
                        , const int _port
   );

   const bool listen(
                          const int _socket
                        , const int _backlog = SOMAXCONN
   );

   const ssize_t send(
                          const int _socket
                        , const void* _buffer
                        , const size_t _size
                        , const int _flags = 0
   );

   const ssize_t recv(
                          const int _socket
                        , void* const _buffer
                        , const size_t _size
                        , const int _flags = 0
   );

   const int accept(
                          const int _socket
                        , sockaddr* const _address = nullptr
                        , socklen_t* const _address_len = nullptr
   );

   const bool select(
                          const int _max_socket
                        , fd_set* const _fd_set_read
                        , fd_set* const _fd_set_write = nullptr
                        , fd_set* const _fd_set_except = nullptr
                        , timeval* const _timeout = nullptr
   );

   void close( int _socket );

} // namespace base::socket



namespace base {

   class Socket
   {
   protected:
      enum class eRead : size_t { OK, ERROR, DISCONNECTED };

   public:
      Socket( const int socket_family, const int socket_type, const int socket_protocole, const char* server_address, const int server_port, const size_t buffer_capacity );
      ~Socket( );

      void init( );
      void close( );
      void print( const std::string& message ) const;
      const bool create( );
      const bool bind( );
      const bool connect( );
      const bool listen( );
      const int accept( );
      const bool select( const int max_socket, fd_set* const fd_set_read, fd_set* const fd_set_write, fd_set* const fd_set_except, timeval* const timeout = nullptr );
      const bool send( const void* p_buffer, const size_t size, const int flags = 0 );
      const bool recv( const int flags = 0 );

   private:
      void init_un( );
      void init_in( );

   private:
      int            m_socket_family = 0;
      int            m_socket_type = 0;
      int            m_socket_protocole = 0;
      const char*    m_server_address = nullptr;
      int            m_server_port = 0;

   public:
      const int socket( ) const;
   protected:
      int            m_socket = -1;

   private:
      int            m_last_errno = 0;
      sockaddr*      mp_sockaddr = nullptr;
      size_t         m_sockaddr_size = 0;

   public:
      const void* const buffer( size_t& ) const;
   private:
      void fill_buffer( const char symbol = 0 );
   private:
      void*          mp_buffer = nullptr;
      size_t         m_buffer_capacity = 0;
      size_t         m_buffer_size = 0;
   };

}



namespace base {

   class Client: public Socket
   {
   public:
      Client( const int socket_family, const int socket_type, const int socket_protocole, const char* server_address, const int server_port, const size_t buffer_capacity );
      ~Client( );
   };

}



#include <set>



namespace base {

   class Server: public Socket
   {
   public:
      Server( const int socket_family, const int socket_type, const int socket_protocole, const char* server_address, const int server_port, const size_t buffer_capacity );
      ~Server( );

   public:
      const bool select( );

   public:

   private:
      void fd_set_reset( );
      void fd_set_init( );
   private:
      fd_set* mp_fd_set_read;
      fd_set* mp_fd_set_write;
      fd_set* mp_fd_set_except;

   private:
      std::set< int > m_slave_sockets_set;
      int m_max_socket = -1;
   };

}
