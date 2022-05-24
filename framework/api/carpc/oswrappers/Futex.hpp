#pragma once

#include "carpc/oswrappers/Types.hpp"
#include "carpc/oswrappers/linux/kernel.hpp"



namespace carpc::os {

   class Futex
   {
      public:
         class AutoLocker
         {
            public:
               AutoLocker( os::Futex& futex )
                  : m_futex( futex )
               {
                  m_futex.lock( );
               }
               ~AutoLocker( )
               {
                  m_futex.unlock( );
               }

            private:
               os::Futex& m_futex;
         };

      public:
         using tFutex = int;
         const tFutex LOCKED = 0;
         const tFutex UNLOCKED = 1;

      public:
         Futex( const bool auto_lock = false );
         Futex( const Futex& ) = delete;
         ~Futex( );

      public:
         const futex::ID& id( ) const;
      protected:
         futex::ID   m_id = futex::ID::generate( );

      public:
         bool lock( );
         bool unlock( );
      protected:
         tFutex      m_futex = UNLOCKED;
         int         m_error = 0;
   };



   inline
   const futex::ID& Futex::id( ) const
   {
      return m_id;
   }

} // namespace carpc::os



#if 0 // Example

   #include <vector>
   #include <memory>
   #include "carpc/oswrappers/Futex.hpp"
   #include "carpc/oswrappers/Thread.hpp"



   carpc::os::Futex futex;

   void thread_loop( const std::size_t index )
   {
      MSG_INF( "enter: %zu", index );

      std::size_t count = 0;
      while( true )
      {
         futex.lock( );
         printf( "\n" );

         printf( "%zu %zu %zu %zu %zu ", index, index, index, index, index );
         printf( "%zu %zu %zu %zu %zu ", index, index, index, index, index );
         printf( "%zu %zu %zu %zu %zu ", index, index, index, index, index );
         printf( "%zu %zu %zu %zu %zu ", index, index, index, index, index );
         printf( "%zu %zu %zu %zu %zu ", index, index, index, index, index );
         printf( "%zu %zu %zu %zu %zu ", index, index, index, index, index );
         printf( "%zu %zu %zu %zu %zu ", index, index, index, index, index );
         printf( "%zu %zu %zu %zu %zu ", index, index, index, index, index );
         printf( "%zu %zu %zu %zu %zu ", index, index, index, index, index );

         printf( "\n" );
         futex.unlock( );
      }

      MSG_INF( "exit: %zu", index );
   }

   int main( int argc, char** argv, char** envp )
   {
      std::vector< std::shared_ptr< carpc::os::Thread > > threads;
      for( std::size_t index = 0; index < 10; ++index )
      {
         std::shared_ptr< carpc::os::Thread > p_thread( new carpc::os::Thread( thread_loop, index ) );
         threads.push_back( p_thread );
         p_thread->run( );
      }

      for( auto& p_thread : threads )
      {
         p_thread->join( );
      }

      return 0;
   }

#endif