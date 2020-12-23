// Framework
#include "api/sys/comm/async/runnable/Runnable.hpp"
// Application
#include "imp/app/events/AppEvent.hpp"
#include "imp/app/components/OnOff/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOff"



using namespace application::components::onoff;



base::component::IComponent::tSptr Component::creator( base::application::IThread& service )
{
   return std::shared_ptr< Component >( new Component( service, "OnOff" ) );
}

Component::Component( base::application::IThread& _service, const std::string& _name )
   : base::component::RootComponent( _service, _name )
   , m_server_onoff( "OnOffService", "OnOffService-Server" )
   , m_timer( this, _name )
   , m_fsm( )
{
   DBG_MSG( "Created: %s", name( ).c_str( ) );
   m_fsm.run( );
}

Component::~Component( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
}

void Component::boot( const std::string& command )
{
   DBG_MSG( "%s", command.c_str( ) );
   // sleep(3);

   // const base::Timer::ID id = base::timer::start( 3000, 1, [ this ]( const base::Timer::ID id ){ on_timer( id ); } );
   // DBG_MSG( "started timer: %s", id.name( ).c_str( ) );
}

void Component::process_timer( const base::Timer::ID id )
{
   DBG_MSG( "Timer '%s' expired", id.name( ).c_str( ) );
   if( id == m_timer.id( ) )
   {
      DBG_WRN( "Shutting down system" );
      shutdown( );
   }
}

void Component::on_timer( const base::Timer::ID id )
{
   DBG_MSG( "Timer expired: %s", id.name( ).c_str( ) );

   auto operation = [ this ]( )
   {
      DBG_MSG( "operation" );
      events::AppEvent::Event::create_send( { events::eAppEventID::BOOT }, { "booting" } );
      m_timer.start( 20000000000, 1 );
   };
   base::async::Runnable::create_send( operation );
}
