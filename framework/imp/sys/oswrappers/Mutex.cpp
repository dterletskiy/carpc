#include "api/sys/oswrappers/Mutex.hpp"



using namespace base::os;



Mutex::Mutex( const bool auto_lock, const char* const name )
   : m_name( name )
   , m_auto_lock( auto_lock )
{
   pthread_mutexattr_init( &m_attr );
   pthread_mutexattr_settype( &m_attr, PTHREAD_MUTEX_RECURSIVE );
   pthread_mutex_init( &m_mutex, &m_attr );

   // printf( "%d: created", m_id );
   if( m_auto_lock ) lock( );
}

Mutex::~Mutex( )
{
   // printf( "%d: destroyed", m_id );
   if( m_locked ) unlock( );
}

void Mutex::lock( )
{
   pthread_mutex_lock( &m_mutex );

   m_locked = true;
   // printf( "%d: locked", m_id );
}

void Mutex::unlock( )
{
   pthread_mutex_unlock( &m_mutex );

   m_locked = false;
   // printf( "%d: unlocked", m_id );
}



MutexAutoLocker::MutexAutoLocker( Mutex& mutex )
   : m_mutex( mutex )
{
   m_mutex.lock( );
}

MutexAutoLocker::~MutexAutoLocker( )
{
   m_mutex.unlock( );
}
