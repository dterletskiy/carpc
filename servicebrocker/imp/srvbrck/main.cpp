#include "api/sys/tools/Tools.hpp"
#include "ConnectionProcessor.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "MAIN"



using tStart = void (*)( int, char**, char** );
using tExit = void (*)( void );

void preinit( int argc, char** argv, char** envp )
{
   base::tools::PCE configuration( argc, argv, envp );
   configuration.print( );

   std::string string_trace_strategy = configuration.value( "trace" ).value_or( "CONSOLE" );
   base::trace::eLogStrategy trace_strategy = base::trace::eLogStrategy::CONSOLE;
   if( "CONSOLE" == string_trace_strategy )  trace_strategy = base::trace::eLogStrategy::CONSOLE;
   else if( "DLT" == string_trace_strategy ) trace_strategy = base::trace::eLogStrategy::DLT;
   else                                      trace_strategy = base::trace::eLogStrategy::CONSOLE;
   base::trace::Logger::init( trace_strategy, "SBR" );

   MSG_INF( "preinit_array" );
}
__attribute__(( section(".preinit_array") )) tStart __preinit__ = preinit;

void init( int argc, char** argv, char** envp )
{
   MSG_INF( "init_array" );
}
__attribute__(( section(".init_array") )) tStart __init__ = init;

void fini( )
{
   MSG_INF( "fini_array" );
}
__attribute__(( section(".fini_array") )) tExit __fini__ = fini;

void __constructor__( ) __attribute__(( constructor(101) ));
void __constructor__( )
{
   MSG_INF( "constructor" );
}

void __destructor__( ) __attribute__(( destructor(101) ));
void __destructor__( )
{
   MSG_INF( "destructor" );
}



int main( int argc, char** argv, char** envp )
{
   base::tools::PCE configuration( argc, argv, envp );
   configuration.print( );

   std::string address = configuration.value( "ipc_servicebrocker_address" ).value( );
   int port = static_cast< int >( std::stoll( configuration.value( "ipc_servicebrocker_port" ).value( ) ) );
   int protocole = static_cast< int >( std::stoll( configuration.value( "ipc_servicebrocker_protocole" ).value( ) ) );
   base::os::os_linux::socket::configuration socket_configuration{ AF_UNIX, SOCK_STREAM, protocole, address, port };

   size_t buffer_size = static_cast< size_t >( std::stoll( configuration.value( "ipc_servicebrocker_buffer_size" ).value( ) ) );

   ConnectionProcessor conn( socket_configuration, buffer_size );
   conn.init( );
   conn.connection_loop( );



   return 0;
}
