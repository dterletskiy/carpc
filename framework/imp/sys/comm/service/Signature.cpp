#include "api/sys/comm/service/Signature.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "SERVICE_SIGNATURE"



using namespace base::service;



Signature::Signature( const base::async::tAsyncTypeID& type_id, const std::string& role )
   : m_type_id( type_id )
   , m_role( role )
{ }

Signature::Signature( const Signature& other )
   : m_type_id( other.m_type_id )
   , m_role( other.m_role )
{ }

bool Signature::to_stream( base::ipc::tStream& stream ) const
{
   return stream.push( m_type_id, m_role );
}

bool Signature::from_stream( base::ipc::tStream& stream )
{
   return stream.pop( m_type_id, m_role );
}

Signature& Signature::operator=( const Signature& other )
{
   if( other == *this )
      return *this;

   m_type_id = other.m_type_id;
   m_role = other.m_role;

   return *this;
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

const std::string Signature::name( ) const
{
   return m_type_id.name( ) + "." + m_role;
}
