#include "api/sys/comm/event/Types.hpp"


namespace base::async {

   const char* c_str( const eAsyncType comm_type )
   {
      switch( comm_type )
      {
         case eAsyncType::EVENT:       return "base::eAsyncType::EVENT";
         case eAsyncType::RUNNABLE:    return "base::eAsyncType::RUNNABLE";
         default:                      return "base::eAsyncType::UNEFINED";
      }
   }

   const char* c_str( const eCommType comm_type )
   {
      switch( comm_type )
      {
         case eCommType::ETC:    return "base::eCommType::ETC";
         case eCommType::ITC:    return "base::eCommType::ITC";
         case eCommType::IPC:    return "base::eCommType::IPC";
         case eCommType::NONE:   return "base::eCommType::NONE";
         default:                return "base::eCommType::UNEFINED";
      }
   }

}



using namespace base::async;



Address::Address( const base::ID& internal_id, const base::ID& external_id )
   : m_internal_id( internal_id )
   , m_external_id( external_id )
{
}

Address::Address( const Address& other )
   : m_internal_id( other.m_internal_id )
   , m_external_id( other.m_external_id )
{
}

bool Address::to_stream( base::dsi::tByteStream& stream ) const
{
   return stream.push( m_internal_id, m_external_id );
}

bool Address::from_stream( base::dsi::tByteStream& stream )
{
   return stream.pop( m_internal_id, m_external_id );
}

Address& Address::operator=( const Address& other )
{
   m_internal_id = other.m_internal_id;
   m_external_id = other.m_external_id;
   return *this;
}

bool Address::operator==( const Address& other ) const
{
   return ( other.m_internal_id == m_internal_id ) && ( other.m_external_id == m_external_id );
}

bool Address::operator!=( const Address& other ) const
{
   return !( *this == other );
}

bool Address::operator<( const Address& other ) const
{
   if( base::InvalidID != m_external_id && base::InvalidID != other.m_external_id )
      if( other.m_external_id != m_external_id )
         return m_external_id < other.m_external_id;
   return m_internal_id < other.m_internal_id;
}

Address::operator bool( ) const
{
   return base::InvalidID != m_internal_id;
}

const std::string Address::name( ) const
{
   return base::format_string( std::hex, m_external_id, ".", m_internal_id );
}

bool Address::is_local( ) const
{
   return base::InvalidID == m_external_id;
}

bool Address::is_external( ) const
{
   return base::InvalidID != m_external_id;
}

const base::ID& Address::external_id( ) const
{
   return m_external_id;
}

const base::ID& Address::internal_id( ) const
{
   return m_internal_id;
}
