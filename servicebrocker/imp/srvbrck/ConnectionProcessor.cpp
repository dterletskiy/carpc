#include "api/sys/oswrappers/linux/kernel.hpp"
#include "ConnectionProcessor.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "ConnProc"



ConnectionProcessor::ConnectionProcessor( )
   : base::os::SocketServer(
         base::configuration::dsi::service_brocker,
         base::configuration::dsi::buffer_size
      )
{
}

ConnectionProcessor::~ConnectionProcessor( )
{
}

bool ConnectionProcessor::init( )
{
   if( eResult::ERROR == create( ) )
      return false;
   if( eResult::ERROR == bind( ) )
      return false;
   unblock( );
   if( eResult::ERROR == listen( ) )
      return false;
   info( "Connection created" );

   return true;
}

bool ConnectionProcessor::shutdown( )
{
   return true;
}

void ConnectionProcessor::read_slave( base::os::Socket::tSptr p_socket )
{
   size_t size = 0;
   const void* const p_buffer = p_socket->buffer( size );
   p_socket->send( p_buffer, size );
}

void ConnectionProcessor::connection_loop( )
{
   while( true )
   {
      select( );
   }
}
