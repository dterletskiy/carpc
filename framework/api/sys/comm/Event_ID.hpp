#pragma once

#include "api/sys/common/Types.hpp"
#include "api/sys/common/ByteBufferT.hpp"



namespace base {



class Event_ID
{
public:
   Event_ID( const std::string& );
   ~Event_ID( );

   bool to_buffer( ByteBufferT& ) const;
   bool from_buffer( ByteBufferT& );

public:
   const bool operator==( const Event_ID& ) const;
   const bool operator!=( const Event_ID& ) const;
   const bool operator>( const Event_ID& ) const;
   const bool operator<( const Event_ID& ) const;

public:
   const char* c_str( ) const;
private:
   std::string    m_id;
};



} // namespace base
