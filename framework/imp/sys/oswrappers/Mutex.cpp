#include "api/sys/helpers/functions/print.hpp"
#include "api/sys/oswrappers/Mutex.hpp"



using namespace base::os;



// #define DEBUG
#ifdef DEBUG
   #define MESSAGE( FORMAT, ... ) base::write( FORMAT, ##__VA_ARGS__ );
#else
   #define MESSAGE( FORMAT, ... )
#endif



Mutex::Mutex( const bool auto_lock, const bool recursive )
{
   pthread_mutexattr_init( &m_mutex_attr );
   if( true == recursive )
      pthread_mutexattr_settype( &m_mutex_attr, PTHREAD_MUTEX_RECURSIVE );
   pthread_mutex_init( &m_mutex, &m_mutex_attr );

   MESSAGE( "%s: created(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );

   if( auto_lock )
      lock( );
}

Mutex::~Mutex( )
{
   MESSAGE( "%s: destroyed(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );

   if( m_locked )
      unlock( );

   pthread_mutex_destroy( &m_mutex );
   pthread_mutexattr_destroy( &m_mutex_attr );
}

bool Mutex::lock( )
{
   MESSAGE( "%s: trying to lock(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
   const int result = pthread_mutex_lock( &m_mutex );
   m_error = errno;
   if( 0 != result )
   {
      MESSAGE( "%s: lock(%#10lx) error: %d\n", m_id.name( ).c_str( ), pthread_self( ), m_error );
      return false;
   }

   m_locked = true;
   MESSAGE( "%s: locked(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
   return true;
}

bool Mutex::try_lock( )
{
   MESSAGE( "%s: trying to lock(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
   const int result = pthread_mutex_trylock( &m_mutex );
   m_error = errno;
   if( 0 != result )
   {
      MESSAGE( "%s: lock(%#10lx) error: %d\n", m_id.name( ).c_str( ), pthread_self( ), m_error );
      return false;
   }

   m_locked = true;
   MESSAGE( "%s: locked(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
   return true;
}

bool Mutex::unlock( )
{
   MESSAGE( "%s: trying to unlock(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
   const int result = pthread_mutex_unlock( &m_mutex );
   m_error = errno;
   if( 0 != result )
   {
      MESSAGE( "%s: unlock(%#10lx) error: %d\n", m_id.name( ).c_str( ), pthread_self( ), m_error );
      return false;
   }

   m_locked = false;
   MESSAGE( "%s: unlocked(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
   return true;
}
