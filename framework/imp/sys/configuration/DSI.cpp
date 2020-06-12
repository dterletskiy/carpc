#include <sys/socket.h>
#include "api/sys/configuration/DSI.hpp"



#define UNIX_SOCKET
// #define INET_SOCKET


namespace base::configuration::dsi {

#if defined UNIX_SOCKET

   const os::linux::socket::configuration service_brocker = {
      AF_UNIX, SOCK_STREAM, 0, "/tmp/servicebrocker.socket", 50000
   };
   const os::linux::socket::configuration application = {
      AF_UNIX, SOCK_STREAM, 0, "/tmp/application.socket", 50001
   };

#elif defined INET_SOCKET

   const os::linux::socket::configuration service_brocker = {
      AF_INET, SOCK_STREAM, 0, "127.0.0.1", 50000
   };

#endif

   const size_t buffer_size = 4096;

}
