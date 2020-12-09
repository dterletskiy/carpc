#include "imp/experimental/api/onoff/Types.hpp"



namespace api::onoff {

   const fw::event::tID convert( const eEventID id )
   {
      return static_cast< fw::event::tID >( id );
   }

}


using namespace api::onoff;



DEFINE_EVENT( Event );