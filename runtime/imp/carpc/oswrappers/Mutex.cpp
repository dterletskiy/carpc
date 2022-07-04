#include "carpc/helpers/functions/print.hpp"
#include "carpc/oswrappers/Mutex.hpp"



using namespace carpc::os;



// #define DEBUG
#ifdef DEBUG
   #define MESSAGE( FORMAT, ... ) carpc::write( FORMAT, ##__VA_ARGS__ );
#else
   #define MESSAGE( FORMAT, ... )
#endif



Mutex::Mutex( const bool auto_lock, const bool recursive )
{
   pthread_mutexattr_init( &m_mutex_attr );
   if( true == recursive )
      pthread_mutexattr_settype( &m_mutex_attr, PTHREAD_MUTEX_RECURSIVE );
   pthread_mutex_init( &m_mutex, &m_mutex_attr );

   MESSAGE( "%s: created(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );

   if( auto_lock )
      lock( );
}

Mutex::~Mutex( )
{
   if( 0 != m_locked )
   {
      MESSAGE( "%s: destroying locked mutex(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   }

   pthread_mutex_destroy( &m_mutex );
   pthread_mutexattr_destroy( &m_mutex_attr );

   MESSAGE( "%s: destroyed(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
}

bool Mutex::lock( )
{
   MESSAGE( "%s: trying to lock(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   const int result = pthread_mutex_lock( &m_mutex );
   m_error = errno;
   if( 0 != result )
   {
      MESSAGE( "%s: lock(%#10lx) error: %d\n", m_id.dbg_name( ).c_str( ), pthread_self( ), m_error );
      return false;
   }

   ++m_locked;
   MESSAGE( "%s: locked(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   return true;
}

bool Mutex::try_lock( )
{
   MESSAGE( "%s: trying to lock(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   const int result = pthread_mutex_trylock( &m_mutex );
   m_error = errno;
   if( 0 != result )
   {
      MESSAGE( "%s: lock(%#10lx) error: %d\n", m_id.dbg_name( ).c_str( ), pthread_self( ), m_error );
      return false;
   }

   ++m_locked;
   MESSAGE( "%s: locked(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   return true;
}

bool Mutex::timed_lock( const timespec& abs_timeout )
{
   MESSAGE( "%s: trying to timed lock(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   const int result = pthread_mutex_timedlock( &m_mutex, &abs_timeout );
   m_error = errno;
   if( 0 != result )
   {
      MESSAGE( "%s: timed lock(%#10lx) error: %d\n", m_id.dbg_name( ).c_str( ), pthread_self( ), m_error );
      return false;
   }

   ++m_locked;
   MESSAGE( "%s: timed locked(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   return true;
}

bool Mutex::unlock( )
{
   MESSAGE( "%s: trying to unlock(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   const int result = pthread_mutex_unlock( &m_mutex );
   m_error = errno;
   if( 0 != result )
   {
      MESSAGE( "%s: unlock(%#10lx) error: %d\n", m_id.dbg_name( ).c_str( ), pthread_self( ), m_error );
      return false;
   }

   --m_locked;
   MESSAGE( "%s: unlocked(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   return true;
}



MutexRW::MutexRW( )
{
   pthread_rwlockattr_init( &m_mutex_attr );
   pthread_rwlock_init( &m_mutex, &m_mutex_attr );

   MESSAGE( "%s: created(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
}

MutexRW::~MutexRW( )
{
   if( 0 != m_locked )
   {
      MESSAGE( "%s: destroying locked mutex(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   }

   pthread_rwlock_destroy( &m_mutex );
   pthread_rwlockattr_destroy( &m_mutex_attr );

   MESSAGE( "%s: destroyed(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
}

bool MutexRW::read_lock( )
{
   MESSAGE( "%s: trying to read lock(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   const int result = pthread_rwlock_rdlock( &m_mutex );
   m_error = errno;
   if( 0 != result )
   {
      MESSAGE( "%s: read lock(%#10lx) error: %d\n", m_id.dbg_name( ).c_str( ), pthread_self( ), m_error );
      return false;
   }

   ++m_locked;
   MESSAGE( "%s: read locked(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   return true;
}

bool MutexRW::write_lock( )
{
   MESSAGE( "%s: trying to write lock(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   const int result = pthread_rwlock_wrlock( &m_mutex );
   m_error = errno;
   if( 0 != result )
   {
      MESSAGE( "%s: write lock(%#10lx) error: %d\n", m_id.dbg_name( ).c_str( ), pthread_self( ), m_error );
      return false;
   }

   ++m_locked;
   MESSAGE( "%s: write locked(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   return true;
}

bool MutexRW::try_read_lock( )
{
   MESSAGE( "%s: trying to read lock(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   const int result = pthread_rwlock_tryrdlock( &m_mutex );
   m_error = errno;
   if( 0 != result )
   {
      MESSAGE( "%s: read lock(%#10lx) error: %d\n", m_id.dbg_name( ).c_str( ), pthread_self( ), m_error );
      return false;
   }

   ++m_locked;
   MESSAGE( "%s: read locked(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   return true;
}

bool MutexRW::read_time_lock( const timespec& abs_timeout )
{
   MESSAGE( "%s: trying to timed read lock(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   const int result = pthread_rwlock_timedrdlock( &m_mutex, &abs_timeout );
   m_error = errno;
   if( 0 != result )
   {
      MESSAGE( "%s: timed read lock(%#10lx) error: %d\n", m_id.dbg_name( ).c_str( ), pthread_self( ), m_error );
      return false;
   }

   ++m_locked;
   MESSAGE( "%s: timed read locked(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   return true;
}

bool MutexRW::write_time_lock( const timespec& abs_timeout )
{
   MESSAGE( "%s: trying to timed write lock(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   const int result = pthread_rwlock_timedwrlock( &m_mutex, &abs_timeout );
   m_error = errno;
   if( 0 != result )
   {
      MESSAGE( "%s: timed write lock(%#10lx) error: %d\n", m_id.dbg_name( ).c_str( ), pthread_self( ), m_error );
      return false;
   }

   ++m_locked;
   MESSAGE( "%s: timed write locked(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   return true;
}

bool MutexRW::try_write_lock( )
{
   MESSAGE( "%s: trying to write lock(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   const int result = pthread_rwlock_trywrlock( &m_mutex );
   m_error = errno;
   if( 0 != result )
   {
      MESSAGE( "%s: write lock(%#10lx) error: %d\n", m_id.dbg_name( ).c_str( ), pthread_self( ), m_error );
      return false;
   }

   ++m_locked;
   MESSAGE( "%s: write locked(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   return true;
}

bool MutexRW::unlock( )
{
   MESSAGE( "%s: trying to unlock(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   const int result = pthread_rwlock_unlock( &m_mutex );
   m_error = errno;
   if( 0 != result )
   {
      MESSAGE( "%s: unlock(%#10lx) error: %d\n", m_id.dbg_name( ).c_str( ), pthread_self( ), m_error );
      return false;
   }

   --m_locked;
   MESSAGE( "%s: unlocked(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   return true;
}
