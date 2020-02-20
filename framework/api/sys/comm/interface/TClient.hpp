#pragma once

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TClient"



namespace base {



template< typename TYPES >
class TClient
{
public:
   TClient( );
   ~TClient( );

public:
   virtual void connected( ) = 0;
   virtual void disconnected( ) = 0;

public:
   virtual void process_response_event( const typename TYPES::tEvent& ) = 0;
   virtual void process_notification_event( const typename TYPES::tEvent& ) = 0;
};



template< typename TYPES >
TClient< TYPES >::TClient( )
{
}

template< typename TYPES >
TClient< TYPES >::~TClient( )
{
}


} // namespace base



#undef CLASS_ABBR
