#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/component/RootComponent.hpp"
#include "api/sys/events/Events.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "RootComponent"



using namespace base::component;



RootComponent::RootComponent( application::IThread& app_thread, const std::string& name )
   : Component( app_thread, name )
{
   SYS_INF( "%p", this );
}

RootComponent::~RootComponent( )
{
}

void RootComponent::boot( const std::string& message )
{
}

void RootComponent::shutdown( const std::string& message )
{
   events::system::System::Event::create_send(
      { events::system::eID::shutdown },
      { "shutdown application" }
   );
}
