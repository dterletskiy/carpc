#include <sys/socket.h>

#pragma once



// const int socket_family = AF_UNIX;
const int socket_family = AF_INET;

const int socket_type = SOCK_STREAM;
// const int socket_type = SOCK_SEQPACKET;
const int socket_protocole = 0;

const int server_port = 5000;

const char* server_address = "/tmp/9Lq7BNBnBycd6nxy.socket";
const char* inet_address = "localhost";

