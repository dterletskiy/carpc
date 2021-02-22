#include "api/sys/tools/Tools.hpp"
#include "ConnectionProcessor.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "MAIN"



namespace memory {
   #ifdef HOOK_MEMORY_ALLOCATOR
      extern void dump( );
   #else
      inline void dump( ) { }
   #endif
}

void __constructor__( ) __attribute__(( constructor(101) ));
void __constructor__( )
{
   MSG_INF( "starting..." );
   base::trace::Logger::init( base::trace::eLogStrategy::CONSOLE, "SBR" );
}

void __destructor__( ) __attribute__(( destructor(101) ));
void __destructor__( )
{
   MSG_INF( "finishing..." );
}



int main( int argc, char *argv[] )
{
   base::tools::cmd::init( argc, argv );
   base::tools::cmd::print( );
   base::tools::cfg::init( base::tools::cmd::argument( "config" ).value_or( "servicebrocker.cfg" ) );
   base::tools::cfg::print( );

   std::string address = base::tools::cfg::argument( "ipc_servicebrocker_address" ).value( );
   int port = static_cast< int >( std::stoll( base::tools::cfg::argument( "ipc_servicebrocker_port" ).value( ) ) );
   int protocole = static_cast< int >( std::stoll( base::tools::cfg::argument( "ipc_servicebrocker_protocole" ).value( ) ) );
   base::os::linux::socket::configuration configuration{ AF_UNIX, SOCK_STREAM, protocole, address, port };

   size_t buffer_size = static_cast< size_t >( std::stoll( base::tools::cfg::argument( "ipc_servicebrocker_buffer_size" ).value( ) ) );

   ConnectionProcessor conn( configuration, buffer_size );
   conn.init( );
   conn.connection_loop( );



   return 0;
}
