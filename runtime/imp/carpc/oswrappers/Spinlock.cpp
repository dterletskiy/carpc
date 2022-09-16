#include "carpc/helpers/functions/print.hpp"
#include "carpc/oswrappers/Spinlock.hpp"



using namespace carpc::os;



// #define DEBUG
#ifdef DEBUG
   #define MESSAGE( FORMAT, ... ) carpc::write( FORMAT, ##__VA_ARGS__ );
#else
   #define MESSAGE( FORMAT, ... )
#endif



thread_local int Spinlock::m_error{ 0 };

Spinlock::Spinlock( const bool auto_lock, const bool shared )
{
   pthread_spin_init( &m_spinlock, shared ? PTHREAD_PROCESS_SHARED : PTHREAD_PROCESS_PRIVATE );

   if( auto_lock )
      lock( );

   MESSAGE( "%s: created(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
}

Spinlock::~Spinlock( )
{
   unlock( );

   pthread_spin_destroy( &m_spinlock );

   MESSAGE( "%s: destroyed(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
}

bool Spinlock::lock( )
{
   MESSAGE( "%s: trying to lock(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   const int result = pthread_spin_lock( &m_spinlock );
   m_error = errno;
   if( 0 != result )
   {
      MESSAGE( "%s: lock(%#10lx) error: %d\n", m_id.dbg_name( ).c_str( ), pthread_self( ), m_error );
      return false;
   }

   MESSAGE( "%s: locked(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   return true;
}

bool Spinlock::try_lock( )
{
   MESSAGE( "%s: trying to lock(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   const int result = pthread_spin_trylock( &m_spinlock );
   m_error = errno;
   if( 0 != result )
   {
      MESSAGE( "%s: lock(%#10lx) error: %d\n", m_id.dbg_name( ).c_str( ), pthread_self( ), m_error );
      return false;
   }

   MESSAGE( "%s: locked(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   return true;
}

bool Spinlock::unlock( )
{
   MESSAGE( "%s: trying to unlock(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   const int result = pthread_spin_unlock( &m_spinlock );
   m_error = errno;
   if( 0 != result )
   {
      MESSAGE( "%s: unlock(%#10lx) error: %d\n", m_id.dbg_name( ).c_str( ), pthread_self( ), m_error );
      return false;
   }

   MESSAGE( "%s: unlocked(%#10lx)\n", m_id.dbg_name( ).c_str( ), pthread_self( ) );
   return true;
}
