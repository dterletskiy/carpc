#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>

#pragma once



// const int socket_family = AF_UNIX;
const int socket_family = AF_INET;

const int socket_type = SOCK_STREAM;
// const int socket_type = SOCK_SEQPACKET;
const int socket_protocole = 0;

const int server_port = 5000;

const char* server_address = "/tmp/9Lq7BNBnBycd6nxy.socket";
const char* inet_address = "localhost";

