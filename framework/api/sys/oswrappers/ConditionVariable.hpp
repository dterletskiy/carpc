#pragma once

#include "api/sys/oswrappers/linux/thread.hpp"
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
         ID                m_id;

      public:
         void lock( );
         void unlock( );
         void wait( );
         void notify( const bool all = true );
      private:
         pthread_mutex_t   m_mutex = PTHREAD_MUTEX_INITIALIZER;
         pthread_cond_t    m_cond_var = PTHREAD_COND_INITIALIZER;
   };



   inline
   const ID ConditionVariable::id( ) const
   {
      return m_id;
   }

} // namespace base::os
