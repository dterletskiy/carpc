#pragma once

#include <set>

#include "api/sys/oswrappers/Socket.hpp"
#include "api/sys/common/Types.hpp"
#include "api/sys/common/ByteBufferT.hpp"
#include "api/sys/configuration/DSI.hpp"



class ConnectionProcessor
{
   enum class eRead : size_t { OK, ERROR, DISCONNECTED };

public:
   ConnectionProcessor( );
   ~ConnectionProcessor( );

   bool init( const int, const int, const int, const char*, const int );
   bool shutdown( );
   void connection_loop( );
   int execute( );

private:
   void process_slave_sockets( );
   eRead read_slave_socket( const base::os::linux::socket::tSocket );
private:
   base::os::linux::socket::tSocket m_master_socket = -1;
   std::set< base::os::linux::socket::tSocket > m_slave_sockets_set;
   base::os::linux::socket::tSocket m_max_socket = -1;

private:
   void fd_set_reset( );
   base::os::linux::socket::tSocket fd_set_init( );
private:
   fd_set m_fd_set_read;
   fd_set m_fd_set_write;
   fd_set m_fd_set_except;

private:
   void* mp_buffer = nullptr;
};
