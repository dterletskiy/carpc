#include "api/sys/command/Types.hpp"



namespace carpc::command::queue {

   const char* c_str( const eState& state )
   {
      switch( state )
      {
         case eState::IDLE:      return "IDLE";
         case eState::RUNNING:   return "RUNNING";
         case eState::PAUSING:   return "PAUSING";
         case eState::PAUSED:    return "PAUSED";
         case eState::RESUMING:  return "RESUMING";
         case eState::STOPPING:  return "STOPPING";
         case eState::STOPED:    return "STOPED";
         default:                return "UNEFINED";
      }
      return "UNDEFINED";
   }

}



namespace carpc::command::command {

   const char* c_str( const eState& state )
   {
      switch( state )
      {
         case eState::IDLE:      return "IDLE";
         case eState::EXECUTING: return "EXECUTING";
         case eState::PAUSING:   return "PAUSING";
         case eState::PAUSED:    return "PAUSED";
         case eState::RESUMING:  return "RESUMING";
         case eState::STOPPING:  return "STOPPING";
         case eState::STOPED:    return "STOPED";
         case eState::FINISHED:  return "FINISHED";
         default:                return "UNEFINED";
      }
      return "UNDEFINED";
   }

}
