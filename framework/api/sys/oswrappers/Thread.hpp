#pragma once

#include "api/sys/common/Types.hpp"
#include "api/sys/oswrappers/Types.hpp"



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

   static const TID current_id( );
   static const std::uint64_t convert( const TID& );

public:
   const TID& id( ) const;
private:
   TID                        m_id;

public:
   const pthread_attr_t& attr( ) const;
private:
   pthread_attr_t             m_attr;

private:
   FunctionThread             mp_function;
   bool                       m_created = false;
   static void* thread_loop( void* );
};
using ThreadPtr = std::shared_ptr< base::os::Thread >;



template< typename _Fn, typename... _Args >
Thread::Thread( _Fn&& p_function, _Args&&... args )
   : mp_function( )
{
   pthread_attr_init( &m_attr );
   pthread_attr_setdetachstate( &m_attr, PTHREAD_CREATE_JOINABLE );
   pthread_attr_setscope( &m_attr,PTHREAD_SCOPE_PROCESS );

   mp_function = std::bind( p_function, args... );
}

inline
const TID& Thread::id( ) const
{
   return m_id;
}

inline
const pthread_attr_t& Thread::attr( ) const
{
   return m_attr;
}




} // namespace base::os


