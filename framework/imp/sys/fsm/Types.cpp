#include "api/sys/fsm/Types.hpp"



namespace carpc::fsm {

   const char* c_str( const eSignal& signal )
   {
      switch( signal )
      {
         case eSignal::SYSTEM:   return "SYSTEM";
         case eSignal::STATE:    return "STATE";
         default:                return "UNEFINED";
      }
   }

}