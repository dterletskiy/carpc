#pragma once

#include "comm/dsi/Stub.hpp"



namespace application::api::onoff {



class OnOffStub;
using OnOffStubPtr = std::shared_ptr< OnOffStub >;

class OnOffStub
   : public base::Stub
{
public:
   static OnOffStubPtr create_stub( const std::string& );

public:
   ~OnOffStub( );
private:
   OnOffStub( const std::string& );
};



} // namespace application::api::onoff
