#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/events/ServiceEvent.hpp"
#include "api/sys/component/RootComponent.hpp"
#include "imp/sys/component/SysEventConsumer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "RootComponent"


namespace base {



RootComponent::RootComponent( const ServicePtr p_service, const std::string& name )
   : Component( p_service, name )
{
   SYS_INF( "%p", this );
   SysEventConsumer::instance( this );
}

RootComponent::~RootComponent( )
{
}

void RootComponent::exit( ) const
{
   ServiceEvent::send_event( { eServiceCommand::shutdown, "shutdown" }, eCommType::ITC );
}


} // namespace base
