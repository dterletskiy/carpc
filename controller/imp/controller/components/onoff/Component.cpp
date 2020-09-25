// Framework
#include "api/sys/comm/event/Runnable.hpp"
// Application
#include "imp/controller/components/onoff/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOff"



using namespace controller::components::onoff;



base::IComponent::tSptr Component::creator( base::IServiceThread& service )
{
   return std::shared_ptr< Component >( new Component( service, "OnOff" ) );
}

Component::Component( base::IServiceThread& service, const std::string& name )
   : base::RootComponent( service, name )
   , m_server_onoff( )
   , m_client_onoff( )
{
   DBG_MSG( "Created: %s", base::Component::name( ).c_str( ) );
}

Component::~Component( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
}

void Component::boot( const std::string& command )
{
   DBG_MSG( "%s", command.c_str( ) );

   const base::ID id = base::timer::start( 60000, 1, [ this ]( const base::ID id ){ on_timer( id ); } );
   DBG_MSG( "started timer: %zu", id );
}

void Component::on_timer( const base::ID id )
{
   DBG_MSG( "Timer expired: %zu", id );

   DBG_WRN( "Shutting down system" );
   shutdown( );
}
