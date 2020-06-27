#pragma once

#include <set>

#include "api/sys/oswrappers/Socket.hpp"
#include "api/sys/common/Types.hpp"
#include "api/sys/configuration/DSI.hpp"



class ConnectionProcessor : public base::os::SocketServer
{
   enum class eRead : size_t { OK, ERROR, DISCONNECTED };

public:
   ConnectionProcessor( );
   ~ConnectionProcessor( );

   bool init( );
   bool shutdown( );
   void connection_loop( );
   int execute( );

private:
   void read_slave( base::os::Socket::tSptr ) override;
};
