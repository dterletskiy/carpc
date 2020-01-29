// Application
#include "imp/app/components/server/Server.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffServer"



namespace application::onoff {



Server::Server( const std::string& role_name )
   : api::onoff::Server( role_name )
   , m_timer( this )
{
   // DBG_MSG( "Created" );
}

Server::~Server( )
{
   // DBG_MSG( "Destroyed" );
}

void Server::request_trigger_state( const std::string& state, const size_t delay )
{
   DBG_MSG( "state: %s / delay: %zu", state.c_str( ), delay );
   m_timer.start( delay );
   
}

void Server::process_timer( const base::TimerID id )
{
   DBG_MSG( "Timer '%#lx' expired", (long)id );

   if( id == m_timer.id( ) )
   {
      response_trigger_state( true );
      m_timer.stop( );
   }
}



} // namespace application::onoff
