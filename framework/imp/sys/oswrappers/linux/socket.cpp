#include <sys/ioctl.h>
#include <net/if.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

#include "api/sys/oswrappers/linux/socket.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Socket"



namespace base::os::linux {



bool get_mac( const std::string& connection_name, std::string& connection_mac )
{
   int fd = socket( AF_UNIX, SOCK_DGRAM, IPPROTO_IP );
   int errno_fd = errno;
   SYS_INF( "socket descriptor: %d", fd );
   if( -1 == fd )
   {
      SYS_ERR( "error: %d", errno_fd );
      return false;
   }

   struct ifreq ifreq_data;
   strcpy( ifreq_data.ifr_name, connection_name.c_str( ) );
   int result = ioctl( fd, SIOCGIFHWADDR, &ifreq_data );
   int errno_ioctl = errno;
   SYS_INF( "ioctl result: %d", result );
   if( 0 != result )
   {
      SYS_ERR( "error: %d", errno_ioctl );
      return false;
   }

   connection_mac.clear( );
   for( size_t i = 0; i < 6; ++i )
   {
      char buffer[3];
      sprintf( buffer, "%02x", ( uint8_t )ifreq_data.ifr_addr.sa_data[i] );
      connection_mac += buffer;
   }
   SYS_INF( "Connection: %s / MAC: %s", connection_name.c_str( ), connection_mac.c_str( ) );

   return true;
}



} // namespace base::os::linux

