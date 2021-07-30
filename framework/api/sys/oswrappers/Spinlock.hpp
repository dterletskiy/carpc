#pragma once

#include "api/sys/oswrappers/linux/thread.hpp"
#include "api/sys/common/ID.hpp"



namespace base::os {

   class Spinlock
   {
      public:
         class AutoLocker
         {
            public:
               AutoLocker( os::Spinlock& spinlock )
                  : m_spinlock( spinlock )
               {
                  m_spinlock.lock( );
               }
               ~AutoLocker( )
               {
                  m_spinlock.unlock( );
               }

            private:
               os::Spinlock& m_spinlock;
         };

      public:
         using ID = base::TID< Spinlock >;

      public:
         Spinlock( const bool auto_lock = false, const bool shared = false );
         Spinlock( const Spinlock& ) = delete;
         ~Spinlock( );

      public:
         const ID id( ) const;
      protected:
         ID                      m_id = ID::generate( );

      public:
         bool lock( );
         bool try_lock( );
         bool unlock( );
      protected:
         bool                    m_locked = false;
         int                     m_error = 0;

      protected:
         pthread_spinlock_t      m_spinlock;
   };



   inline
   const Spinlock::ID Spinlock::id( ) const
   {
      return m_id;
   }

} // namespace base::os
