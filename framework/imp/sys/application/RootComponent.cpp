#include "api/sys/application/RootComponent.hpp"
#include "api/sys/events/Events.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "RootComponent"



using namespace carpc::application;



RootComponent::RootComponent( const std::string& name )
   : IComponent( name )
{
   SYS_DBG( "Created: %s", m_name.c_str( ) );
}

RootComponent::~RootComponent( )
{
   SYS_DBG( "Destroyed: %s", m_name.c_str( ) );
}

void RootComponent::shutdown( const std::string& message )
{
   events::system::System::Event::create_send(
      { events::system::eID::shutdown },
      { "shutdown application" }
   );
}
