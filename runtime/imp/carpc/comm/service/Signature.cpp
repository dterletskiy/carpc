#include "carpc/comm/service/Signature.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "SERVICE_SIGNATURE"



using namespace carpc::service;



Signature::Signature( const carpc::async::tAsyncTypeID& type_id, const std::string& role )
   : m_type_id( type_id )
   , m_role( role )
{ }

Signature::Signature( const Signature& other )
   : m_type_id( other.m_type_id )
   , m_role( other.m_role )
{ }

bool Signature::to_stream( carpc::ipc::tStream& stream ) const
{
   return ipc::serialize( stream, m_type_id, m_role );
}

bool Signature::from_stream( carpc::ipc::tStream& stream )
{
   return ipc::deserialize( stream, m_type_id, m_role );
}

bool Signature::operator==( const Signature& other ) const
{
   return ( m_type_id == other.m_type_id ) && ( m_role == other.m_role );
}

bool Signature::operator!=( const Signature& other ) const
{
   return !( *this == other );
}

bool Signature::operator<( const Signature& other ) const
{
   if( other.m_type_id != m_type_id )
      return m_type_id < other.m_type_id;

   return m_role < other.m_role;
}

const std::string Signature::dbg_name( ) const
{
   return m_type_id.dbg_name( ) + "." + m_role;
}
