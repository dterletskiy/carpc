// Framework
#include "api/sys/comm/async/runnable/Runnable.hpp"
// Application
#include "imp/core/components/onoff/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOff"



namespace core::event {

   const char* c_str( const eID id )
   {
      switch( id )
      {
         case eID::boot:         return "core::event::eID::boot";
         case eID::shutdown:     return "core::event::eID::shutdown";
         case eID::ping:         return "core::event::eID::ping";
         case eID::undefined:
         default:                return "core::event::eID::undefined";
      }
   }

}



using namespace core::components::onoff;



base::component::IComponent::tSptr Component::creator( base::application::IThread& service )
{
   return std::shared_ptr< Component >( new Component( service, "OnOff" ) );
}

Component::Component( base::application::IThread& _service, const std::string& _name )
   : base::component::RootComponent( _service, _name )
   , m_timer( this )
   , m_server_onoff( )
{
   MSG_DBG( "Created: %s", name( ).c_str( ) );
   core::event::App::Event::set_notification( this, { core::event::eID::boot } );
}

Component::~Component( )
{
   MSG_DBG( "Destroyed: %s", name( ).c_str( ) );
}

void Component::boot( const std::string& command )
{
   MSG_DBG( "%s", command.c_str( ) );

   REGISTER_EVENT( core::event::App );
   // core::event::App::Event::create_send( { core::event::eID::boot },
   //    base::application::Context( base::application::process::ID::generate( ), base::application::thread::ID::generate( ) )
   // );
   // core::event::App::Event::create_send( { core::event::eID::boot } );
}

void Component::process_timer( const base::Timer::ID id )
{
   MSG_DBG( "Timer '%s' expired", id.name( ).c_str( ) );
   if( id == m_timer.id( ) )
   {
      MSG_WRN( "Shutting down system" );
      shutdown( );
   }
}

void Component::on_timer( const base::Timer::ID id )
{
   MSG_DBG( "Timer expired: %s", id.name( ).c_str( ) );

   MSG_WRN( "Preparing shutdown system" );
   m_timer.start( 5000000000, 1 );
}

void Component::process_event( const core::event::App::Event& event )
{
   MSG_DBG( "%s", event.signature( )->name( ).c_str( ) );

   const base::Timer::ID id = base::timer::start( 100000, 1, [ this ]( const base::Timer::ID id ){ on_timer( id ); } );
   MSG_DBG( "started timer: %s", id.name( ).c_str( ) );
}
