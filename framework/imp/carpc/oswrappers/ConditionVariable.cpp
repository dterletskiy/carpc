#include "carpc/helpers/functions/print.hpp"
#include "carpc/oswrappers/ConditionVariable.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "OS_CONDVAR"



using namespace carpc::os;



// #define DEBUG
#ifdef DEBUG
   #define MESSAGE( FORMAT, ... ) carpc::write( FORMAT, ##__VA_ARGS__ );
#else
   #define MESSAGE( FORMAT, ... )
#endif



ConditionVariable::ConditionVariable( )
   : Mutex( false, false )
{
   pthread_condattr_init( &m_cond_attr );
   pthread_cond_init( &m_cond_var, &m_cond_attr );

   MESSAGE( "%s: created(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
}

ConditionVariable::~ConditionVariable( )
{
   pthread_cond_destroy( &m_cond_var );
   pthread_condattr_destroy( &m_cond_attr );

   MESSAGE( "%s: destroyed(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
}

bool ConditionVariable::wait( )
{
   m_is_cond_var.store( false );

   MESSAGE( "%s: waiting(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
   const int result = pthread_cond_wait( &m_cond_var, &m_mutex );
   m_error = errno;
   if( 0 != result )
   {
      MESSAGE( "%s: waiting(%#10lx) error: %d\n", m_id.name( ).c_str( ), pthread_self( ), m_error );
      return false;
   }

   MESSAGE( "%s: wait finished(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
   return true;
}

bool ConditionVariable::notify( const bool all )
{
   m_is_cond_var.store( true );

   int result = 0;
   if( all )
   {
      MESSAGE( "%s: notify all(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
      result = pthread_cond_broadcast( &m_cond_var );
      m_error = errno;
   }
   else
   {
      MESSAGE( "%s: notify one(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
      result = pthread_cond_signal( &m_cond_var );
      m_error = errno;
   }

   if( 0 != result )
   {
      MESSAGE( "%s: notify(%#10lx) error: %d\n", m_id.name( ).c_str( ), pthread_self( ), m_error );
      return false;
   }

   return true;
}

bool ConditionVariable::test( ) const
{
   return m_is_cond_var.load( );
}
