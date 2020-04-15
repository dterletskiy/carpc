#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/component/RootComponent.hpp"
#include "imp/sys/events/Events.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "RootComponent"


namespace base {



RootComponent::RootComponent( const IServiceThread::tSptr p_service, const std::string& name )
   : Component( p_service, name )
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
   events::service::ServiceEvent::Event::create_send( { events::service::eID::shutdown }, { "shutdown application" }, eCommType::ITC );
}


} // namespace base
