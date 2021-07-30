#pragma once

#include "api/sys/oswrappers/linux/thread.hpp"
#include "api/sys/common/ID.hpp"



namespace base::os {

   class Mutex
   {
      public:
         class AutoLocker
         {
            public:
               AutoLocker( os::Mutex& mutex )
                  : m_mutex( mutex )
               {
                  m_mutex.lock( );
               }
               ~AutoLocker( )
               {
                  m_mutex.unlock( );
               }

            private:
               os::Mutex& m_mutex;
         };

      public:
         using ID = base::TID< Mutex >;

      public:
         Mutex( const bool auto_lock = false, const bool recursive = true );
         Mutex( const Mutex& ) = delete;
         ~Mutex( );

      public:
         const ID id( ) const;
      protected:
         ID                   m_id = ID::generate( );

      public:
         bool lock( );
         bool try_lock( );
         bool unlock( );
      protected:
         bool                 m_locked = false;
         int                  m_error = 0;

      protected:
         pthread_mutex_t      m_mutex = PTHREAD_MUTEX_INITIALIZER;
         pthread_mutexattr_t  m_mutex_attr;
   };



   inline
   const Mutex::ID Mutex::id( ) const
   {
      return m_id;
   }

} // namespace base::os
