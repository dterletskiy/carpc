#pragma once

#include "api/sys/oswrappers/Types.hpp"



namespace base::os {



class Mutex
{
public:
   Mutex( const bool auto_lock = false, const char* name = "mutex_no_name" );
   Mutex( const Mutex& ) = delete;
   virtual ~Mutex( );

public:
   const char* const name( ) const;
private:
   const char* const m_name;

public:
   void lock( );
   void unlock( );
private:
   bool        m_locked = false;
   bool        m_auto_lock = false;

public:
   pthread_mutex_t& mutex( );
private:
   pthread_mutex_t   m_mutex = PTHREAD_MUTEX_INITIALIZER;
};



inline
const char* const Mutex::name( ) const
{
   return m_name;
}

inline
pthread_mutex_t& Mutex::mutex( )
{
   return m_mutex;
}



class MutexAutoLocker
{
public:
   MutexAutoLocker( Mutex& );
   ~MutexAutoLocker( );

private:
   Mutex& m_mutex;
};



} // namespace base::os
