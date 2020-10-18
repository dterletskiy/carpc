#pragma once

#include <string>
#include <list>
#include <functional>
#include <memory>
#include "api/sys/oswrappers/linux/kernel.hpp"
#include "api/sys/oswrappers/linux/socket.hpp"
#include "api/sys/common/Includes.hpp"
#include "api/sys/common/ID.hpp"



namespace base::os {

   class Socket
   {
      public:
         enum class eResult { OK, ERROR, DISCONNECTED };
         const char* c_str( const eResult );

         using ID = base::TID< Socket >;
         using tList = std::list< Socket >;
         using tSptr = std::shared_ptr< Socket >;
         using tWptr = std::weak_ptr< Socket >;
         using tSptrList = std::list< tSptr >;
         using tSptrSet = std::set< tSptr >;
         using tRecvCallback = std::function< void( void*, const size_t, const eResult ) >;

      public:
         Socket( const linux::socket::configuration&, const size_t );
         Socket( const Socket& ) = delete;
         Socket( Socket&& );
         ~Socket( );
      protected:
         Socket( linux::socket::tSocket, const size_t );

      public:
         const bool operator<( const Socket& ) const;
         Socket& operator=( const Socket& ) = delete;
         Socket& operator=( Socket&& );

      public:
         eResult create( );
         eResult bind( );
         eResult listen( );
         tSptr accept( );
         eResult connect( );
         eResult send( const void* p_buffer, const size_t size, const int flags = 0 );
         eResult recv( const int flags = 0 );
         eResult recv( tRecvCallback, const int flags = 0 );
         void close( );
         void info( const std::string& message ) const;

      public:
         void block( ) const;
         void unblock( ) const;

      public:
         linux::socket::tSocket socket( ) const;
         bool is_valid( ) const;
      protected:
         linux::socket::tSocket        m_socket = linux::socket::InvalidSocket;

      public:
         ID id( ) const;
      private:
         ID                            m_id;

      public:
         const linux::socket::configuration& configuration( ) const;
      protected:
         linux::socket::configuration  m_configuration = { };

      public:
         const void* const buffer( size_t& ) const;
      protected:
         void fill_buffer( const char symbol = 0 );
      protected:
         void*                         mp_buffer = nullptr;
         size_t                        m_buffer_capacity = 0;
         size_t                        m_buffer_size = 0;
         size_t                        m_total_recv_size = 0;
         size_t                        m_total_send_size = 0;
   };



   inline
   linux::socket::tSocket Socket::socket( ) const
   {
      return m_socket;
   }

   inline
   bool Socket::is_valid( ) const
   {
      return linux::socket::InvalidSocket != m_socket;
   }

   inline
   Socket::ID Socket::id( ) const
   {
      return m_id;
   }

   inline
   void Socket::block( ) const
   {
   }

   inline
   void Socket::unblock( ) const
   {
      linux::set_nonblock( m_socket );
   }

   inline
   const linux::socket::configuration& Socket::configuration( ) const
   {
      return m_configuration;
   }

}



namespace base::os {

   class SocketClient: public Socket
   {
      public:
         SocketClient( const linux::socket::configuration&, const size_t );
         ~SocketClient( );
   };

}



namespace base::os {

   class SocketServer: public Socket
   {
      public:
         SocketServer( const linux::socket::configuration&, const size_t );
         ~SocketServer( );

      public:
         bool select( );

      private:
         virtual void read_slave( tSptr );
         void fd_reset( );
         void fd_init( );

      public:
         linux::socket::tSocket max_socket( ) const;
      private:
         void calc_max( );
         linux::socket::tSocket     m_max_socket = -1;

      private:
         tSptrList                  m_slave_sockets;
         linux::socket::fd          m_fd;
   };



   inline
   linux::socket::tSocket SocketServer::max_socket( ) const
   {
      return m_max_socket;
   }

}
