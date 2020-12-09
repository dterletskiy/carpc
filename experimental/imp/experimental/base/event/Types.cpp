#include "imp/experimental/base/event/Types.hpp"



using namespace fw::event;



bool Info::operator<( const Info& other ) const
{
   if( class_id != other.class_id )
      return class_id < other.class_id;
   return id < other.id;
}

const std::string Info::to_string( ) const
{
   return class_id + "." + std::to_string( id );
}
