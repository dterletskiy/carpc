#pragma once

#include "api/sys/oswrappers/linux/thread.hpp"
#include "api/sys/common/Includes.hpp"
#include "api/sys/common/ID.hpp"



namespace base::os {

   class Thread
   {
      public:
         using ID = base::TID< Thread >;
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

         static const ID current_id( );
         static const linux::thread::tID current_thread_id( );

      public:
         const ID& id( ) const;
      private:
         ID                         m_id = ID::generate( );

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
   const Thread::ID& Thread::id( ) const
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

   inline
   const std::string& Thread::name( ) const
   {
      return m_name;
   }

} // namespace base::os


