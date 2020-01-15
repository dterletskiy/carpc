#pragma once

#include "comm/dsi/Proxy.hpp"



namespace application::api::onoff {



class OnOffProxy;
using OnOffProxyPtr = std::shared_ptr< OnOffProxy >;

class OnOffProxy
   : public base::Proxy
{
public:
   static OnOffProxyPtr create_proxy( const std::string& );

public:
   ~OnOffProxy( );
private:
   OnOffProxy( const std::string& );
};



} // namespace application::api::onoff
