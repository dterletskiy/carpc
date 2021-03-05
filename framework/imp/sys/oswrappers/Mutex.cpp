#include "api/sys/helpers/functions/print.hpp"
#include "api/sys/oswrappers/Mutex.hpp"



using namespace base::os;



// #define DEBUG
#ifdef DEBUG
   #define MESSAGE( FORMAT, ... ) base::write( FORMAT, ##__VA_ARGS__ );
#else
   #define MESSAGE( FORMAT, ... )
#endif



Mutex::Mutex( const bool auto_lock, const bool recursive, const char* const name )
   : m_name( name )
{
   pthread_mutexattr_init( &m_attr );
   if( true == recursive )
      pthread_mutexattr_settype( &m_attr, PTHREAD_MUTEX_RECURSIVE );
   pthread_mutex_init( &m_mutex, &m_attr );

   MESSAGE( "%s: created(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );

   if( auto_lock )
      lock( );
}

Mutex::~Mutex( )
{
   MESSAGE( "%s: destroyed(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
   if( m_locked )
      unlock( );
}

void Mutex::lock( )
{
   MESSAGE( "%s: trying to lock(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
   pthread_mutex_lock( &m_mutex );

   m_locked = true;
   MESSAGE( "%s: locked(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
}

void Mutex::unlock( )
{
   pthread_mutex_unlock( &m_mutex );

   m_locked = false;
   MESSAGE( "%s: unlocked(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
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
