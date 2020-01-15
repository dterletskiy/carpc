#pragma once

#include "api/sys/common/Types.hpp"



namespace base::os {



class ConditionVariable
{
public:
   ConditionVariable( );
   ConditionVariable( const ConditionVariable& ) = delete;
   virtual ~ConditionVariable( );

public:
   const ID id( ) const;
private:
   ID          m_id;

public:
   void lock( );
   void unlock( );
   void wait( );
   void notify( const bool all = true );
private:
   #if OS == STD
      std::mutex                       m_mutex;
      std::map<
           std::thread::id
         , std::pair< std::unique_lock< std::mutex >, bool >
      >                                m_lockers_map;
      std::condition_variable          m_cond_var;
      bool                             m_notified = false;
   #elif OS == LINUX
      pthread_mutex_t                  m_mutex = PTHREAD_MUTEX_INITIALIZER;
      pthread_cond_t                   m_cond_var = PTHREAD_COND_INITIALIZER;
   #endif
};



inline
const ID ConditionVariable::id( ) const
{
   return m_id;
}



} // namespace base::os
