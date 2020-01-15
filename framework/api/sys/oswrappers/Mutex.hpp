#pragma once

#include "api/sys/common/Types.hpp"



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
   #if OS == STD
      std::mutex& mutex( );
   #elif OS == LINUX
      pthread_mutex_t& mutex( );
   #elif OS == WINDOWS
   #endif
private:
   #if OS == STD
      std::mutex        m_mutex;
   #elif OS == LINUX
      pthread_mutex_t   m_mutex = PTHREAD_MUTEX_INITIALIZER;
   #elif OS == WINDOWS
   #endif
};



inline
const char* const Mutex::name( ) const
{
   return m_name;
}

inline
#if OS == STD
   std::mutex& Mutex::mutex( )
#elif OS == LINUX
   pthread_mutex_t& Mutex::mutex( )
#elif OS == WINDOWS
#endif
{
   return m_mutex;
}



} // namespace base::os
