#include "api/sys/comm/Event_ID.hpp"


namespace base {



Event_ID::Event_ID( const std::string& id )
   : m_id( id )
{
}

Event_ID::~Event_ID( )
{
}

bool Event_ID::to_buffer( ByteBufferT& buffer ) const
{
   return buffer.push( m_id );
}

bool Event_ID::from_buffer( ByteBufferT& buffer )
{
   return buffer.pop( m_id );
}

const char* Event_ID::c_str( ) const
{
   return m_id.c_str( );
}

const bool Event_ID::operator==( const Event_ID& id ) const
{
   return m_id == id.m_id;
}

const bool Event_ID::operator!=( const Event_ID& id ) const
{
   return m_id != id.m_id;
}

const bool Event_ID::operator>( const Event_ID& id ) const 
{
   return m_id > id.m_id;
}

const bool Event_ID::operator<( const Event_ID& id ) const 
{
   return m_id < id.m_id;
}



} // namespace base
