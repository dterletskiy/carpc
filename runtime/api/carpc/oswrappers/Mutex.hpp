#pragma once

#include "carpc/oswrappers/Types.hpp"
#include "carpc/oswrappers/linux/thread.hpp"



namespace carpc::os {

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
         Mutex( const bool auto_lock = false, const bool recursive = true );
         Mutex( const Mutex& ) = delete;
         ~Mutex( );

      public:
         const mutex::ID& id( ) const;
      protected:
         mutex::ID            m_id = mutex::ID::generate( );

      public:
         bool lock( );
         bool try_lock( );
         bool timed_lock( const timespec& abs_timeout );
         bool unlock( );
      protected:
         unsigned int         m_locked = 0;
         int                  m_error = 0;

      protected:
         pthread_mutex_t      m_mutex = PTHREAD_MUTEX_INITIALIZER;
         pthread_mutexattr_t  m_mutex_attr;
   };



   inline
   const mutex::ID& Mutex::id( ) const
   {
      return m_id;
   }

} // namespace carpc::os



namespace carpc::os {

   class MutexRW
   {
      public:
         MutexRW( );
         MutexRW( const MutexRW& ) = delete;
         ~MutexRW( );

      public:
         const mutex::ID& id( ) const;
      protected:
         mutex::ID            m_id = mutex::ID::generate( );

      public:
         bool read_lock( );
         bool write_lock( );
         bool try_read_lock( );
         bool try_write_lock( );
         bool read_time_lock( const timespec& abs_timeout );
         bool write_time_lock( const timespec& abs_timeout );
         bool unlock( );
      protected:
         unsigned int         m_locked = 0;
         int                  m_error = 0;

      protected:
         pthread_rwlock_t     m_mutex = PTHREAD_RWLOCK_INITIALIZER;
         pthread_rwlockattr_t m_mutex_attr;
   };



   inline
   const mutex::ID& MutexRW::id( ) const
   {
      return m_id;
   }

} // namespace carpc::os
