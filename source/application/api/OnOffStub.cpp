#include "OnOff.hpp"
#include "OnOffStub.hpp"



namespace application::api::onoff {



OnOffStub::OnOffStub( const std::string& _service_name )
   : base::Stub( s_interface_name, _service_name )
{
}

OnOffStub::~OnOffStub( )
{
}

OnOffStubPtr OnOffStub::create_stub( const std::string& _service_name )
{
   OnOffStubPtr stub_ptr = OnOffStubPtr( new OnOffStub( _service_name ) );
   return stub_ptr;
}



} // namespace application::api::onoff
