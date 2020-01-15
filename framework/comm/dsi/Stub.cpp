#include "service/ServiceProcess.hpp"
#include "service/ServiceBrocker.hpp"
#include "Stub.hpp"



namespace base {



Stub::Stub( const std::string& _interface_name, const std::string& _service_name )
   : m_interface_name( _interface_name )
   , m_service_name( _service_name )
{
}

Stub::~Stub( )
{
}



} // namespace base
