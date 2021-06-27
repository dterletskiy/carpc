#include "api/sys/application/IComponent.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IComponent"


using namespace base::application;



IComponent::IComponent( const std::string& name )
   : m_name( name )
{
}
