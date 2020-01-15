#include "api/sys/comm/Types.hpp"


namespace base {



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



EventSignature::EventSignature( const EventTypeID& type_id, const OptEventInfoID& info_id )
   : m_type_id( type_id )
   , m_info_id( info_id )
{ }

EventSignature::EventSignature( const EventSignature& es )
   : m_type_id( es.m_type_id )
   , m_info_id( es.m_info_id )
{ }

bool EventSignature::operator==( const EventSignature& es ) const
{
   if( std::nullopt == m_info_id || std::nullopt == es.m_info_id )
      return m_type_id == es.m_type_id;
   else
      return m_type_id == es.m_type_id && m_info_id == es.m_info_id;
}

bool EventSignature::operator!=( const EventSignature& es ) const
{
   return !( *this == es );
}

bool EventSignature::operator<( const EventSignature& es ) const
{
   if( m_type_id < es.m_type_id ) return true;
   else if( m_info_id < es.m_info_id ) return true;
   else return false;
}

bool EventSignature::operator>( const EventSignature& es ) const
{
   if( m_type_id > es.m_type_id ) return true;
   else if( m_info_id > es.m_info_id ) return true;
   else return true;
}




} // namespace base
