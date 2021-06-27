#pragma once

#include "api/sys/oswrappers/linux/thread.hpp"
#include "api/sys/common/ID.hpp"



namespace base::os {

   class ConditionVariable
   {
      public:
         using ID = base::TID< ConditionVariable >;

      public:
         ConditionVariable( );
         ConditionVariable( const ConditionVariable& ) = delete;
         virtual ~ConditionVariable( );

      public:
         const ID id( ) const;
      private:
         ID                m_id = ID::generate( );

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
   const ConditionVariable::ID ConditionVariable::id( ) const
   {
      return m_id;
   }

} // namespace base::os
