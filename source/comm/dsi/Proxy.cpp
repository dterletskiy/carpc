#include "service/ServiceProcess.hpp"
#include "service/ServiceBrocker.hpp"
#include "Proxy.hpp"



namespace base {



Proxy::Proxy( const std::string& _interface_name, const std::string& _service_name )
   : m_interface_name( _interface_name )
   , m_service_name( _service_name )
{
}

Proxy::~Proxy( )
{
}



} // namespace base
