#pragma once

#include "api/sys/common/Types.hpp"



#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OS_THREAD"

namespace base::os {



class Thread
{
public:
   using FunctionThread = std::function< void( ) >;

public:
   template< typename _Fn, typename... _Args >
      Thread( _Fn&& p_function, _Args&&... args );
   Thread( FunctionThread&& );
   Thread( const Thread& ) = delete;
   virtual ~Thread( );

   bool run( );
   bool join( );

   const TID& id( ) const;
   static const TID current_id( );
   static const std::uint64_t convert( const TID& );

private:
   FunctionThread             mp_function;
   bool                       m_created = false;
   TID                        m_id;
   #if OS == LINUX
      static void* thread_loop( void* );
      pthread_attr_t          m_attr;
   #elif OS == STD
      std::thread             m_thread;
      std::uint64_t           m_ui64id = 0;
   #endif
};
using ThreadPtr = std::shared_ptr< base::os::Thread >;



template< typename _Fn, typename... _Args >
Thread::Thread( _Fn&& p_function, _Args&&... args )
   : mp_function( )
#if OS == STD
   , m_thread{ }
#endif
{
   SYS_INF( );
   #if OS == LINUX
      pthread_attr_init( &m_attr );
      pthread_attr_setdetachstate( &m_attr, PTHREAD_CREATE_JOINABLE );
      pthread_attr_setscope( &m_attr,PTHREAD_SCOPE_PROCESS );
   #endif

   mp_function = std::bind( p_function, args... );
}

inline
const TID& Thread::id( ) const
{
   return m_id;
}




} // namespace base::os

#undef CLASS_ABBR
