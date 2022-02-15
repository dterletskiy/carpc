// Framework
#include "api/sys/comm/async/runnable/Runnable.hpp"
// Application
#include "imp/controller/components/onoff/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOff"



namespace controller::event {

   const char* c_str( const eID id )
   {
      switch( id )
      {
         case eID::boot:         return "controller::event::eID::boot";
         case eID::shutdown:     return "controller::event::eID::shutdown";
         case eID::ping:         return "controller::event::eID::ping";
         case eID::undefined:
         default:                return "controller::event::eID::undefined";
      }
   }

}



using namespace controller::components::onoff;



base::application::IComponent::tSptr Component::creator( )
{
   return std::shared_ptr< Component >( new Component( "OnOff" ) );
}

Component::Component( const std::string& _name )
   : base::application::RootComponent( _name )
   , m_timer( this )
{
   MSG_DBG( "Created: %s", name( ).c_str( ) );
   controller::event::App::Event::set_notification( this, { controller::event::eID::boot } );
}

Component::~Component( )
{
   MSG_DBG( "Destroyed: %s", name( ).c_str( ) );
}

void Component::boot( const std::string& command )
{
   MSG_DBG( "%s", command.c_str( ) );

   REGISTER_EVENT( controller::event::App );
   // controller::event::App::Event::create_send( { controller::event::eID::boot },
   //    base::application::Context( base::application::process::ID::generate( ), base::application::thread::ID::generate( ) )
   // );
   // controller::event::App::Event::create_send( { controller::event::eID::boot },
   //    base::application::Context( base::application::Context::process::broadcast, base::application::Context::thread::broadcast )
   // );
   // controller::event::App::Event::create_send( { controller::event::eID::boot } );
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

void Component::process_event( const controller::event::App::Event& event )
{
   MSG_DBG( "%s", event.signature( )->name( ).c_str( ) );

   const base::Timer::ID id = base::timer::start( 100000, 1, [ this ]( const base::Timer::ID id ){ on_timer( id ); } );
   MSG_DBG( "started timer: %s", id.name( ).c_str( ) );
}