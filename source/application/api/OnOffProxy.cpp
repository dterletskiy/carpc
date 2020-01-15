#include "OnOff.hpp"
#include "OnOffProxy.hpp"



namespace application::api::onoff {



OnOffProxy::OnOffProxy( const std::string& _service_name )
   : base::Proxy( s_interface_name, _service_name )
{
}

OnOffProxy::~OnOffProxy( )
{
}

OnOffProxyPtr OnOffProxy::create_proxy( const std::string& _service_name )
{
   return OnOffProxyPtr( new OnOffProxy( _service_name ) );
}



} // namespace application::api::onoff
