// Application
#include "Server.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffServer"



using namespace controller::components::onoff;



Server::Server( )
   : base::onoff::Server( "OnOff_Controller" )
{
   DBG_MSG( "created" );
   current_state( "UNLOADED" );
}

Server::~Server( )
{
   DBG_MSG( "destroyed" );
}

void Server::connected( )
{
   DBG_MSG( "connected" );
}

void Server::disconnected( )
{
   DBG_MSG( "disconnected" );
}

void Server::request_start( )
{
   DBG_MSG( );
}

void Server::request_trigger_state( const std::string& state, const size_t delay )
{
   DBG_MSG( "state: %s / delay: %zu", state.c_str( ), delay );

   std::shared_ptr< base::Timer > timer = std::make_shared< base::Timer >( this );
   timer->start( delay, 1 );
   m_timers.emplace_back( TimerSeqID{ state, timer, unblock_request( ) } );
   // m_timers.emplace_back( TimerSeqID{ state, timer, 0 } );
}

void Server::process_timer( const base::Timer::ID id )
{
   DBG_MSG( "timer '%s' expired", id.name( ).c_str( ) );

   auto iterator = std::find_if( m_timers.begin( ), m_timers.end( ), [ id ]( const TimerSeqID& element ){ return element.timer->id( ) == id; } );
   if( m_timers.end( ) == iterator )
      return;

   prepare_response( iterator->seq_id );
   response_trigger_state( true );
   current_state( iterator->current_state );
   m_timers.erase( iterator );
}
