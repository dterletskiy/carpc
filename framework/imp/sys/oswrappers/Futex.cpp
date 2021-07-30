#include "api/sys/helpers/functions/print.hpp"
#include "api/sys/oswrappers/Futex.hpp"



using namespace base::os;



// #define DEBUG
#ifdef DEBUG
   #define MESSAGE( FORMAT, ... ) base::write( FORMAT, ##__VA_ARGS__ );
#else
   #define MESSAGE( FORMAT, ... )
#endif



Futex::Futex( const bool auto_lock )
{
   if( auto_lock )
      lock( );

   MESSAGE( "%s: created(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
}

Futex::~Futex( )
{
   if( LOCKED == m_futex )
      unlock( );

   MESSAGE( "%s: destroyed(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
}

bool Futex::lock( )
{
   MESSAGE( "%s: trying to lock(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );

   while( true )
   {
      if( __sync_bool_compare_and_swap( &m_futex, UNLOCKED, LOCKED ) )
         break;

      const int result = os_linux::futex( &m_futex, FUTEX_WAIT | FUTEX_PRIVATE_FLAG, LOCKED, NULL, NULL, 0 );
      m_error = errno;
      if( -1 == result && EAGAIN != m_error )
      {
         MESSAGE( "%s: lock(%#10lx) error: %d\n", m_id.name( ).c_str( ), pthread_self( ), m_error );
         return false;
      }
   }

   MESSAGE( "%s: locked(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
   return true;
}

bool Futex::unlock( )
{
   MESSAGE( "%s: trying to unlock(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );

   if( __sync_bool_compare_and_swap( &m_futex, LOCKED, UNLOCKED ) )
   {
      const int result = os_linux::futex( &m_futex, FUTEX_WAKE | FUTEX_PRIVATE_FLAG, UNLOCKED, NULL, NULL, 0 );
      m_error = errno;
      if ( -1 == result )
      {
         MESSAGE( "%s: unlock(%#10lx) error: %d\n", m_id.name( ).c_str( ), pthread_self( ), m_error );
         return false;
      }
   }

   MESSAGE( "%s: unlocked(%#10lx)\n", m_id.name( ).c_str( ), pthread_self( ) );
   return true;
}
