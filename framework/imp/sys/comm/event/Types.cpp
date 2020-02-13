#include "api/sys/comm/event/Types.hpp"


namespace base {



const SequenceID InvalidSequenceID = 0;

const char* c_str( const eCommType comm_type )
{
   switch( comm_type )
   {
      case eCommType::ETC:    return "eCommType::ETC";
      case eCommType::ITC:    return "eCommType::ITC";
      case eCommType::IPC:    return "eCommType::IPC";
      case eCommType::NONE:   return "eCommType::NONE";
      default:                return "eCommType::UNEFINED";
   }
}

const char* c_str( const eEventType comm_type )
{
   switch( comm_type )
   {
      case eEventType::SIMPLE:      return "eEventType::SIMPLE";
      case eEventType::SIMPLE_ID:   return "eEventType::SIMPLE_ID";
      case eEventType::RR:          return "eEventType::RR";
      default:                      return "eEventType::UNEFINED";
   }
}



} // namespace base
