#include "tools/Tools.hpp"
#include "Mutex.hpp"

namespace base::os {



Mutex::Mutex( const bool auto_lock, const char* name )
   : m_name( name )
   , m_auto_lock( auto_lock )
{
   // printf( "%d: created", m_id );
   if( m_auto_lock ) lock( );
}

Mutex::~Mutex( )
{
   // printf( "%d: destroyed", m_id );
   if( m_auto_lock && m_locked ) unlock( );
}

void Mutex::lock( )
{
   #if OS == STD
      m_mutex.lock( );
   #elif OS == LINUX
      pthread_mutex_lock( &m_mutex );
   #elif OS == WINDOWS
   #endif

   m_locked = true;
   // printf( "%d: locked", m_id );
}

void Mutex::unlock( )
{
   #if OS == STD
      m_mutex.unlock( );
   #elif OS == LINUX
      pthread_mutex_unlock( &m_mutex );
   #elif OS == WINDOWS
   #endif

   m_locked = false;
   // printf( "%d: unlocked", m_id );
}



} // namespace base::os
