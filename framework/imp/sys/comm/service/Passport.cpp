#include "api/sys/comm/service/Passport.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Passport"



using namespace base::service;



Passport::Passport( )
{
}

Passport::Passport( const Signature& _signature, const Address& _address )
   : signature( _signature )
   , address( _address )
{
}

const std::string Passport::name( ) const
{
   return signature.name( ) + " / " + address.name( );
}


bool Passport::to_stream( base::ipc::tStream& stream ) const
{
   return ipc::serialize( stream, signature, address );
}

bool Passport::from_stream( base::ipc::tStream& stream )
{
   return ipc::deserialize( stream, signature, address );
}
