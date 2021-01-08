#include "api/sys/comm/service/Address.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "SERVICE_ADDRESS"



using namespace base::service;



Address::Address( const ID& id )
   : m_context( application::Context::eInitType::Auto )
   , m_id( id )
{
}

Address::Address( const application::Context& context, const ID& id )
   : m_context( context )
   , m_id( id )
{
}

Address::Address( const Address& other )
   : m_context( other.m_context )
   , m_id( other.m_id )
{
}

bool Address::to_stream( base::dsi::tByteStream& stream ) const
{
   return stream.push( m_context, m_id );
}

bool Address::from_stream( base::dsi::tByteStream& stream )
{
   return stream.pop( m_context, m_id );
}

Address& Address::operator=( const Address& other )
{
   if( other == *this )
      return *this;

   m_context = other.m_context;
   m_id = other.m_id;
   return *this;
}

bool Address::operator==( const Address& other ) const
{
   return ( other.m_id == m_id ) && ( other.m_context == m_context );
}

bool Address::operator!=( const Address& other ) const
{
   return !( *this == other );
}

bool Address::operator<( const Address& other ) const
{
   if( other.m_context != m_context )
      return m_context < other.m_context;
   return m_id < other.m_id;
}

const std::string Address::name( ) const
{
   return m_context.name( ) + "." + m_id.name( );
}