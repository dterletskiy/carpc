#include "carpc/comm/service/Passport.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "Passport"



using namespace carpc::service;



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


bool Passport::to_stream( carpc::ipc::tStream& stream ) const
{
   return ipc::serialize( stream, signature, address );
}

bool Passport::from_stream( carpc::ipc::tStream& stream )
{
   return ipc::deserialize( stream, signature, address );
}

bool Passport::operator==( const Passport& other ) const
{
   return ( other.signature == signature ) && ( other.signature == signature );
}

bool Passport::operator!=( const Passport& other ) const
{
   return !( *this == other );
}

bool Passport::operator<( const Passport& other ) const
{
   if( other.signature != signature )
      return signature < other.signature;
   return address < other.address;
}
