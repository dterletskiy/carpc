#include "api/sys/helpers/functions/format.hpp"
#include "api/sys/comm/interface/Types.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "INTERFACE_TYPES"



using namespace base::interface;



Address::Address( const void* ptr, base::ID external_id )
   : m_external_id( external_id )
   , m_ptr( ptr )
{
}

Address::Address( const Address& other )
   : m_external_id( other.m_external_id )
   , m_ptr( other.m_ptr )
{
}

bool Address::to_stream( base::dsi::tByteStream& stream ) const
{
   return stream.push( m_external_id, m_ptr );
}

bool Address::from_stream( base::dsi::tByteStream& stream )
{
   return stream.pop( m_external_id, m_ptr );
}

Address& Address::operator=( const Address& other )
{
   m_external_id = other.m_external_id;
   m_ptr = other.m_ptr;
   return *this;
}

bool Address::operator==( const Address& other ) const
{
   return ( other.m_ptr == m_ptr ) && ( other.m_external_id == m_external_id );
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
   return m_ptr < other.m_ptr;
}

Address::operator bool( ) const
{
   return nullptr != m_ptr;
}

const std::string Address::name( ) const
{
   return base::format_string( std::hex, m_external_id, ".", m_ptr );
}

bool Address::is_local( ) const
{
   return base::InvalidID == m_external_id;
}

bool Address::is_external( ) const
{
   return base::InvalidID != m_external_id;
}

base::ID Address::external_id( ) const
{
   return m_external_id;
}

const void* Address::ptr( ) const
{
   return m_ptr;
}



Signature::Signature( const base::async::tAsyncTypeID& type_id, const std::string& role )
   : m_type_id( type_id )
   , m_role( role )
{ }

Signature::Signature( const Signature& other )
   : m_type_id( other.m_type_id )
   , m_role( other.m_role )
{ }

bool Signature::to_stream( base::dsi::tByteStream& stream ) const
{
   return stream.push( m_type_id, m_role );
}

bool Signature::from_stream( base::dsi::tByteStream& stream )
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
   return m_type_id == other.m_type_id && m_role == other.m_role;
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

const base::async::tAsyncTypeID& Signature::type_id( ) const
{
   return m_type_id;
}

const std::string Signature::role( ) const
{
   return m_role;
}
