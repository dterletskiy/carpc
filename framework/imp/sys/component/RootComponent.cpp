#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/component/RootComponent.hpp"
#include "imp/sys/events/ServiceEvent.hpp"
#include "imp/sys/component/SysEventConsumer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "RootComponent"


namespace base {



RootComponent::RootComponent( const IServiceThread::tSptr p_service, const std::string& name )
   : Component( p_service, name )
{
   SYS_INF( "%p", this );
   mp_consumer = std::shared_ptr< SysEventConsumer >( SysEventConsumer::instance( this ) );
}

RootComponent::~RootComponent( )
{
}

void RootComponent::shutdown( const std::string& info ) const
{
   ServiceEvent::Event::create_send( { eServiceCommand::shutdown, "shutdown" }, eCommType::ITC );
}


} // namespace base
