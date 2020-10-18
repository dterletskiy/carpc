// Framework
#include "api/sys/comm/event/Runnable.hpp"
// Application
#include "imp/hmi/components/onoff/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOff"



namespace hmi::event {

   const char* c_str( const eID id )
   {
      switch( id )
      {
         case eID::boot:         return "hmi::event::eID::boot";
         case eID::shutdown:     return "hmi::event::eID::shutdown";
         case eID::ping:         return "hmi::event::eID::ping";
         case eID::undefined:
         default:                return "hmi::event::eID::undefined";
      }
   }

}



using namespace hmi::components::onoff;



base::component::IComponent::tSptr Component::creator( base::application::IThread& service )
{
   return std::shared_ptr< Component >( new Component( service, "OnOff" ) );
}

Component::Component( base::application::IThread& _service, const std::string& _name )
   : base::component::RootComponent( _service, _name )
   , m_timer( this )
   , m_client_onoff( )
{
   DBG_MSG( "Created: %s", name( ).c_str( ) );
   hmi::event::App::Event::set_notification( this, { hmi::event::eID::boot } );
}

Component::~Component( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
}

void Component::boot( const std::string& command )
{
   DBG_MSG( "%s", command.c_str( ) );

   REGISTER_EVENT( hmi::event::App );
   // hmi::event::App::Event::create_send( { hmi::event::eID::boot },
   //    base::application::Context( base::application::process::ID::generate( ), base::application::thread::ID::generate( ) )
   // );
   // hmi::event::App::Event::create_send( { hmi::event::eID::boot } );
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

   DBG_WRN( "Preparing shutdown system" );
   m_timer.start( 5000000000, 1 );
}

void Component::process_event( const hmi::event::App::Event& event )
{
   DBG_MSG( "%s", event.signature( )->name( ).c_str( ) );

   const base::Timer::ID id = base::timer::start( 100000, 1, [ this ]( const base::Timer::ID id ){ on_timer( id ); } );
   DBG_MSG( "started timer: %s", id.name( ).c_str( ) );
}
