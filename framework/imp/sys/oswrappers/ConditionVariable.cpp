#include "api/sys/helpers/functions/print.hpp"
#include "api/sys/oswrappers/ConditionVariable.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OS_CONDVAR"



using namespace base::os;



// #define DEBUG
#ifdef DEBUG
   #define MESSAGE( FORMAT, ... ) base::write( FORMAT, ##__VA_ARGS__ );
#else
   #define MESSAGE( FORMAT, ... )
#endif



ConditionVariable::ConditionVariable( )
{
   MESSAGE( "%s: created(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
}

ConditionVariable::~ConditionVariable( )
{
   MESSAGE( "%s: destroyed(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
}

void ConditionVariable::lock( )
{
   MESSAGE( "%s: trying to lock(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
   pthread_mutex_lock( &m_mutex );
   MESSAGE( "%s: locked(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
}

void ConditionVariable::unlock( )
{
   pthread_mutex_unlock( &m_mutex );
   MESSAGE( "%s: unlocked(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
}

void ConditionVariable::wait( )
{
   MESSAGE( "%s: waiting(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
   pthread_cond_wait( &m_cond_var, &m_mutex );
   MESSAGE( "%s: wait finished(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
}

void ConditionVariable::notify( const bool all )
{
   if( all )
   {
      MESSAGE( "%s: broadcast(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
      pthread_cond_broadcast( &m_cond_var );
   }
   else
   {
      MESSAGE( "%s: signal(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
      pthread_cond_signal( &m_cond_var );
   }
}
