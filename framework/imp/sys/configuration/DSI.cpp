#include <sys/socket.h>
#include "api/sys/configuration/DSI.hpp"



// #define UNIX_SOCKET
#define INET_SOCKET


namespace base::configuration::dsi {

#if defined UNIX_SOCKET
   const int socket_family = AF_UNIX;
   const char* server_address = "/tmp/9Lq7BNBnBycd6nxy.socket";
#elif defined INET_SOCKET
   const int socket_family = AF_INET;
   const char* server_address = "192.168.6.134";
#endif

   const int socket_type = SOCK_STREAM;
   // const int socket_type = SOCK_SEQPACKET;

   const int socket_protocole = 0;


   const int server_port = 5000;

   const size_t buffer_size = 2048;

}

