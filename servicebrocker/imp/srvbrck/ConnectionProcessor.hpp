#pragma once

#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <set>

#include "api/sys/common/Types.hpp"
#include "api/sys/common/ByteBuffer.hpp"
#include "api/sys/configuration/DSI.hpp"



class ConnectionProcessor
{
public:
   ConnectionProcessor( );
   ~ConnectionProcessor( );

   bool init( const int, const int, const int, const char*, const int );
   bool shutdown( );
   void connection_loop( );
   int execute( );

private:
   int m_master_socket = -1;
   std::set< int > m_slave_sockets_set;
   int m_max_socket = -1;

private:
   void fd_set_reset( );
private:
   fd_set m_fd_set_read;
   fd_set m_fd_set_write;
   fd_set m_fd_set_except;

private:
   void* mp_buffer = nullptr;
   int m_last_errno = 0;
};
