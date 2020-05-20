#pragma once

#include <string>
#include "api/sys/oswrappers/linux/socket.hpp"



namespace base::os {

   class Socket
   {
   public:
      struct Info
      {
         const int socket_family;
         const int socket_type;
         const int socket_protocole;
         const char* server_address;
         const int server_port;
      };
      using tSet = std::set< Socket >;

   protected:
      enum class eRead : size_t { OK, ERROR, DISCONNECTED };

   public:
      Socket( const Info&, const size_t );
      ~Socket( );
   private:
      Socket( linux::socket::tSocket, const size_t );

      bool create( );
      bool bind( );
      bool listen( );
      Socket accept( );
      bool connect( );
      bool send( const void* p_buffer, const size_t size, const int flags = 0 );
      bool recv( const int flags = 0 );
      bool select( linux::socket::tSocket max_socket );
      void close( );
      void info( const std::string& message ) const;

   public:
      linux::socket::tSocket socket( ) const;
   protected:
      linux::socket::tSocket  m_socket = -1;

   protected:
      linux::socket::fd       m_fd;

   protected:
      Info                    m_info = { };

   public:
      const void* const buffer( size_t& ) const;
   protected:
      void fill_buffer( const char symbol = 0 );
   protected:
      void*                   mp_buffer = nullptr;
      size_t                  m_buffer_capacity = 0;
      size_t                  m_buffer_size = 0;
   };

}



namespace base::os {

   class Client: public Socket
   {
   public:
      Client( const Info&, const size_t );
      ~Client( );
   };

}



namespace base::os {

   class Server: public Socket
   {
   public:
      Server( const Info&, const size_t );
      ~Server( );

   private:
      void fd_reset( );
      void fd_init( );

   private:
      tSet                       m_slave_sockets_set;
      linux::socket::tSocket     m_max_socket = -1;
   };

}
