// Application
#include "Server.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffServer"



using namespace application::components::onoff;



Server::Server( const std::string& role_name, const std::string& name )
   : base::onoff::Server( role_name )
   , m_name( name )
{
   DBG_MSG( "%s: created", m_name.c_str( ) );
   current_state( "Unloaded" );
}

Server::~Server( )
{
   DBG_MSG( "%s: destroyed", m_name.c_str( ) );
}

void Server::request_start( )
{
   DBG_MSG( );
}

void Server::request_trigger_state( const std::string& state, const size_t delay )
{
   DBG_MSG( "%s: state: %s / delay: %zu", m_name.c_str( ), state.c_str( ), delay );

   std::shared_ptr< base::Timer > timer = std::make_shared< base::Timer >( this );
   timer->start( delay );
   m_timers.emplace_back( TimerSeqID{ state, timer, unblock_request( ) } );
   // m_timers.emplace_back( TimerSeqID{ state, timer, 0 } );
}

void Server::process_timer( const base::Timer::ID id )
{
   DBG_MSG( "%s: Timer '%#lx' expired", m_name.c_str( ), (long)id );

   auto iterator = std::find_if( m_timers.begin( ), m_timers.end( ), [ id ]( const TimerSeqID& element ){ return element.timer->id( ) == id; } );
   if( m_timers.end( ) == iterator )
      return;

   iterator->timer->stop( );
   prepare_response( iterator->seq_id );
   response_trigger_state( true );
   current_state( iterator->current_state );
   m_timers.erase( iterator );
}
