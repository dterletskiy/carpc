#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/component/RootComponent.hpp"
#include "api/sys/events/Events.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "RootComponent"



using namespace base;



RootComponent::RootComponent( IServiceThread& service, const std::string& name )
   : Component( service, name )
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
   events::service::Service::Event::create_send( { events::service::eID::shutdown }, { "shutdown application" }, base::async::eCommType::ITC );
   // events::service::Service::Event::create_send( { events::service::eID::shutdown }, { "shutdown application" }, base::async::eCommType::IPC );
}
