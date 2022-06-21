#pragma once

#include <atomic>

#include "carpc/oswrappers/Mutex.hpp"



namespace carpc::os {

   class ConditionVariable : public Mutex
   {
      public:
         ConditionVariable( );
         ConditionVariable( const ConditionVariable& ) = delete;
         ~ConditionVariable( );

      public:
         bool wait( );
         bool notify( const bool all = true );
         bool test( ) const;

      private:
         pthread_cond_t       m_cond_var = PTHREAD_COND_INITIALIZER;
         pthread_condattr_t   m_cond_attr;
         std::atomic< bool >  m_is_cond_var = false;
   };

} // namespace carpc::os
