#pragma once

#include <memory>
#include <functional>

#include "carpc/oswrappers/Types.hpp"
#include "carpc/oswrappers/linux/thread.hpp"



namespace carpc::os {

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
         bool run( const std::string& name = "NoName" );
         bool join( );

         static const thread::ID current_id( );
         static const os_linux::thread::tID current_thread_id( );

      public:
         const thread::ID& id( ) const;
      private:
         thread::ID                 m_id = thread::ID::generate( );

      public:
         const os_linux::thread::tID& thread_id( ) const;
      private:
         os_linux::thread::tID      m_thread_id;

      public:
         const pthread_attr_t& attr( ) const;
      private:
         pthread_attr_t             m_attr;

      private:
         FunctionThread             mp_function;
         bool                       m_created = false;
         static void* thread_loop( void* );

      public:
         const std::string& name( ) const;
      private:
         void name( const std::string& );
         std::string                m_name = "NoName";
   };



   template< typename _Fn, typename... _Args >
   Thread::Thread( _Fn&& p_function, _Args&&... args )
      : mp_function( std::bind( p_function, args... ) )
   {
      init( );
   }

   inline
   const thread::ID& Thread::id( ) const
   {
      return m_id;
   }

   inline
   const os_linux::thread::tID& Thread::thread_id( ) const
   {
      return m_thread_id;
   }

   inline
   const pthread_attr_t& Thread::attr( ) const
   {
      return m_attr;
   }

   inline
   const std::string& Thread::name( ) const
   {
      return m_name;
   }

} // namespace carpc::os


