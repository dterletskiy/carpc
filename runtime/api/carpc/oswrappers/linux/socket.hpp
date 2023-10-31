#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <linux/vm_sockets.h>
#include <net/if.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>
#include <set>



namespace carpc::os::os_linux::socket {

   using tSocket = int;
   using tSocketSet = std::set< tSocket >;

   const tSocket InvalidSocket = -1;
   extern thread_local int error;

   const char* const socket_domain_to_string( const int domain );
   int socket_domain_from_string( const char* const domain );

   const char* const socket_type_to_string( const int type );
   int socket_type_from_string( const char* const type );

   struct configuration
   {
      const std::string dbg_name( ) const;
      void print( const std::string& _message = "" ) const;

      bool operator==( const configuration& ) const;
      bool operator!=( const configuration& ) const;

      bool eq( const configuration& ) const;

      int         domain      = -1;
      int         type        = -1;
      int         protocole   = -1;
      std::string address;
      int         port        = -1;
   };

   class socket_addr
   {
      public:
         socket_addr( const int _domain, const char* const _address, const int _port );
         socket_addr( const socket_addr& ) = delete;
         ~socket_addr( );
         socket_addr& operator=( const socket_addr& ) = delete;

      public:
         const sockaddr* const addr( ) const;
         const socklen_t len( ) const;
      private:
         sockaddr* m_addr = nullptr;
         socklen_t m_len = 0;
   };

   class fd
   {
      public:
         enum class eType { READ, WRITE, EXCEPT };

      public:
         fd( );
         ~fd( );

         void set( const tSocket _socket, const eType type );
         void set( const std::set< tSocket > socket_set, const std::set< eType > type_set );
         void clear( const tSocket _socket, const eType type );
         void clear( const std::set< tSocket > socket_set, const std::set< eType > type_set );
         bool is_set( const tSocket _socket, const eType type );
         void reset( );

      private:
         void set( tSocket _socket, std::set< fd_set* > p_fd_set );
         void clear( tSocket _socket, std::set< fd_set* > p_fd_set );
         fd_set* convert( const eType type ) const;
         std::set< fd_set* > convert( const std::set< eType > type_set ) const;

      public:
         fd_set* const read( ) const;
         fd_set* const write( ) const;
         fd_set* const except( ) const;
      private:
         fd_set* mp_read = nullptr;
         fd_set* mp_write = nullptr;
         fd_set* mp_except = nullptr;
   };

} // namespace carpc::os::os_linux::socket



namespace carpc::os::os_linux::socket {

   void print( const sockaddr* _sa );

   void info( const tSocket _socket, const char* _message = "socket" );
   void info( const tSocket _socket, configuration& _config );

   tSocket create_server( const configuration& _config );
   tSocket create_clint( const configuration& _config );

   tSocket socket(
                          const int _domain = AF_UNIX
                        , const int _type = SOCK_STREAM
                        , const int _protocole = 0
   );

   tSocket socket( const configuration _config );

   bool setsockopt(
                          tSocket _socket
                        , int level
                        , int option_name
                        , const void *option_value
                        , socklen_t option_len
   );

   bool bind(
                          const tSocket _socket
                        , const sockaddr* _address
                        , const socklen_t _address_len
   );

   bool bind(
                          const tSocket _socket
                        , const int _domain
                        , const char* const _address
                        , const int _port
   );

   bool bind( const tSocket _socket, const configuration _config );

   bool connect(
                          const tSocket _socket
                        , const sockaddr* _address
                        , const socklen_t _address_len
   );

   bool connect(
                          const tSocket _socket
                        , const int _domain
                        , const char* const _address
                        , const int _port
   );

   bool connect( const tSocket _socket, const configuration _config );

   bool listen(
                          const tSocket _socket
                        , const int _backlog = SOMAXCONN
   );

   ssize_t send(
                          const tSocket _socket
                        , const void* _buffer
                        , const size_t _size
                        , const int _flags = 0
   );

   ssize_t sendto(
                          const tSocket _socket
                        , const void* _buffer
                        , const size_t _size
                        , const int _flags
                        , const sockaddr* const _address
                        , const socklen_t _address_len
   );

   ssize_t sendmsg(
                          const tSocket _socket
                        , const msghdr* const _message
                        , const int _flags = 0
   );

   ssize_t recv(
                          const tSocket _socket
                        , void* const _buffer
                        , const size_t _size
                        , const int _flags = 0
   );

   ssize_t recvfrom(
                          const tSocket _socket
                        , void* const _buffer
                        , const size_t _size
                        , const int _flags
                        , sockaddr* const _address
                        , socklen_t* const _address_len
   );

   ssize_t recvmsg(
                          const tSocket _socket
                        , msghdr* const message
                        , const int _flags = 0
   );

   tSocket accept(
                          const tSocket _socket
                        , sockaddr* const _address = nullptr
                        , socklen_t* const _address_len = nullptr
   );

   bool select(
                          const tSocket _max_socket
                        , fd_set* const _fd_set_read
                        , fd_set* const _fd_set_write = nullptr
                        , fd_set* const _fd_set_except = nullptr
                        , timeval* const _timeout = nullptr
   );

   bool select(
                          const tSocket _max_socket
                        , fd& _fd
                        , timeval* const _timeout = nullptr
   );

   void close( tSocket _socket );

} // namespace carpc::os::os_linux::socket



namespace carpc::os::os_linux {

   bool get_mac( const std::string& connection_name, std::string& connection_mac );

} // namespace carpc::os::os_linux







#if 0

   struct addrinfo
   {
      int              ai_flags;
      int              ai_family;
      int              ai_socktype;
      int              ai_protocol;
      socklen_t        ai_addrlen;
      struct sockaddr *ai_addr;
      char            *ai_canonname;
      struct addrinfo *ai_next;
   };

   struct sockaddr
   {
      sa_family_t sa_family;
      char        sa_data[14];
   };

   #define UNIX_PATH_MAX   108
   struct sockaddr_un
   {
      sa_family_t    sun_family;                   /* address family: AF_UNIX */
      char           sun_path[ UNIX_PATH_MAX ];    /* path */
   };

   struct sockaddr_in
   {
      sa_family_t    sin_family; /* address family: AF_INET */
      in_port_t      sin_port;   /* port in network byte order, e.g. htons(3490) */
      struct in_addr sin_addr;   /* internet address */
   };

   /* Internet address. */
   struct in_addr
   {
      uint32_t       s_addr;     /* address in network byte order, load with inet_aton() */
   };

   struct sockaddr_vm
   {
      sa_family_t    svm_family;    /* Address family: AF_VSOCK */
      unsigned short svm_reserved1;
      unsigned int   svm_port;      /* Port # in host byte order */
      unsigned int   svm_cid;       /* Address in host byte order */
      unsigned char  svm_zero[
                          sizeof(struct sockaddr)
                        - sizeof(sa_family_t)
                        - sizeof(unsigned short)
                        - sizeof(unsigned int)
                        - sizeof(unsigned int)
                     ];
   };

   struct ifreq {
      char ifr_name[IFNAMSIZ]; /* Interface name */
      union
      {
         struct sockaddr ifr_addr;
         struct sockaddr ifr_dstaddr;
         struct sockaddr ifr_broadaddr;
         struct sockaddr ifr_netmask;
         struct sockaddr ifr_hwaddr;
         short           ifr_flags;
         int             ifr_ifindex;
         int             ifr_metric;
         int             ifr_mtu;
         struct ifmap    ifr_map;
         char            ifr_slave[IFNAMSIZ];
         char            ifr_newname[IFNAMSIZ];
         char           *ifr_data;
      };
   };

   struct in6_ifreq
   {
      struct in6_addr     ifr6_addr;
      u32                 ifr6_prefixlen;
      int                 ifr6_ifindex; /* Interface index */
   };

#endif
