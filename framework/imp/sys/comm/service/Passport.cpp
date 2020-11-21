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


bool Passport::to_stream( base::dsi::tByteStream& stream ) const
{
   return stream.push( signature, address );
}

bool Passport::from_stream( base::dsi::tByteStream& stream )
{
   return stream.pop( signature, address );
}
