#pragma once

#include "api/sys/oswrappers/linux/thread.hpp"
#include "api/sys/common/Types.hpp"



namespace base::os {

   class Thread
   {
      public:
         using tSptr = std::shared_ptr< Thread >;
         using FunctionThread = std::function< void( ) >;

      public:
         template< typename _Fn, typename... _Args >
            Thread( _Fn&& p_function, _Args&&... args );
         Thread( FunctionThread&& );
         Thread( const Thread& ) = delete;
         virtual ~Thread( );
      private:
         void init( );

      public:
         bool run( );
         bool join( );

         static const ID current_id( );

      public:
         const ID& id( ) const;
      private:
         ID                         m_id;

      public:
         const linux::thread::tID& thread_id( ) const;
      private:
         linux::thread::tID         m_thread_id;

      public:
         const pthread_attr_t& attr( ) const;
      private:
         pthread_attr_t             m_attr;

      private:
         FunctionThread             mp_function;
         bool                       m_created = false;
         static void* thread_loop( void* );
   };



   template< typename _Fn, typename... _Args >
   Thread::Thread( _Fn&& p_function, _Args&&... args )
      : mp_function( )
   {
      init( );
      mp_function = std::bind( p_function, args... );
   }

   inline
   const ID& Thread::id( ) const
   {
      return m_id;
   }

   inline
   const linux::thread::tID& Thread::thread_id( ) const
   {
      return m_thread_id;
   }

   inline
   const pthread_attr_t& Thread::attr( ) const
   {
      return m_attr;
   }

} // namespace base::os


