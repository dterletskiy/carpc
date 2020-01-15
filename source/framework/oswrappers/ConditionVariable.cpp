#include "tools/Tools.hpp"
#include "Mutex.hpp"
#include "Thread.hpp"
#include "ConditionVariable.hpp"
#include "Trace.hpp"
#define CLASS_ABBR "OS_CONDVAR"

namespace base::os {



ConditionVariable::ConditionVariable( )
   : m_id( tools::generate_id( "cond_var" ) )
#if OS == STD
   , m_mutex( )
   , m_lockers_map( )
   , m_cond_var( )
#endif
{
}

ConditionVariable::~ConditionVariable( )
{
}

void ConditionVariable::lock( )
{
   #if OS == STD
      Mutex auto_lock( true );
      std::thread::id tid = std::this_thread::get_id( );
      auto it = m_lockers_map.find( tid );
      if( it == m_lockers_map.end( ) )
      {
         m_lockers_map.emplace(
               std::make_pair(
                       tid
                     , std::make_pair( std::unique_lock< std::mutex >( m_mutex ), false )
         ) );
      }
      else
      {
         it->second.first.lock( );
      }
   #elif OS == LINUX
      pthread_mutex_lock( &m_mutex );
   #endif
}

void ConditionVariable::unlock( )
{
   #if OS == STD
      Mutex auto_lock( true );
      std::thread::id tid = std::this_thread::get_id( );
      auto it = m_lockers_map.find( tid );
      if( it == m_lockers_map.end( ) ) return;
      it->second.first.unlock( );
   #elif OS == LINUX
      pthread_mutex_unlock( &m_mutex );
   #endif
}

void ConditionVariable::wait( )
{
   #if OS == STD
      Mutex auto_lock( true );
      std::thread::id tid = std::this_thread::get_id( );
      auto it = m_lockers_map.find( tid );
      if( it == m_lockers_map.end( ) ) return;
      auto_lock.unlock( );

      while( false == it->second.second )
         m_cond_var.wait( it->second.first );

      it->second.second = false;
   #elif OS == LINUX
      pthread_cond_wait( &m_cond_var, &m_mutex );
   #endif
}

void ConditionVariable::notify( const bool all )
{
   #if OS == STD
      Mutex auto_lock( true );
      for( auto& pair : m_lockers_map ) pair.second.second = true;
      auto_lock.unlock( );

      if( all )   m_cond_var.notify_all( );
      else        m_cond_var.notify_one( );
   #elif OS == LINUX
      if( all )   pthread_cond_broadcast( &m_cond_var );
      else        pthread_cond_signal( &m_cond_var );
   #endif
}




} // namespace base::os
