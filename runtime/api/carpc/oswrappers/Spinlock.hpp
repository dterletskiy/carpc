#pragma once

#include "carpc/oswrappers/Types.hpp"
#include "carpc/oswrappers/linux/thread.hpp"



namespace carpc::os {

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
         Spinlock( const bool auto_lock = false, const bool shared = false );
         Spinlock( const Spinlock& ) = delete;
         ~Spinlock( );

      public:
         const spinlock::ID& id( ) const;
      protected:
         spinlock::ID               m_id = spinlock::ID::generate( );

      public:
         bool lock( );
         bool try_lock( );
         bool unlock( );
      protected:
         static thread_local int    m_error;

      protected:
         pthread_spinlock_t         m_spinlock;
   };



   inline
   const spinlock::ID& Spinlock::id( ) const
   {
      return m_id;
   }

} // namespace carpc::os
