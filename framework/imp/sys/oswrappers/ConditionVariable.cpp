#include "api/sys/tools/Tools.hpp"
#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/oswrappers/ConditionVariable.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OS_CONDVAR"



using namespace base::os;



ConditionVariable::ConditionVariable( )
   : m_id( tools::id::generate( "cond_var" ) )
{
}

ConditionVariable::~ConditionVariable( )
{
}

void ConditionVariable::lock( )
{
   pthread_mutex_lock( &m_mutex );
}

void ConditionVariable::unlock( )
{
   pthread_mutex_unlock( &m_mutex );
}

void ConditionVariable::wait( )
{
   pthread_cond_wait( &m_cond_var, &m_mutex );
}

void ConditionVariable::notify( const bool all )
{
   if( all )   pthread_cond_broadcast( &m_cond_var );
   else        pthread_cond_signal( &m_cond_var );
}
