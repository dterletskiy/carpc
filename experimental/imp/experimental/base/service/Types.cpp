#include "imp/experimental/base/service/Types.hpp"



using namespace fw::service;



Info::Info( const tServiceID& _id, const tServiceRole& _role )
   : id( _id )
   , role( _role )
{ }

bool Info::operator<( const Info& connection ) const
{
   if( id != connection.id )
      return id < connection.id;
   return role < connection.role;
}

const std::string Info::to_string( ) const
{
   return id + "." + role;
}
