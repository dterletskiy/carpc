#pragma once

#include "api/sys/oswrappers/ConditionVariable.hpp"
#include "api/sys/comm/async/IAsync.hpp"



namespace base::async {

   class AsyncQueue
   {
      public:
         using tSptr = std::shared_ptr< AsyncQueue >;
         using tWptr = std::weak_ptr< AsyncQueue >;

      public:
         AsyncQueue( const std::string& name = "NoName" );
         ~AsyncQueue( );
         AsyncQueue( const AsyncQueue& ) = delete;
         AsyncQueue& operator=( const AsyncQueue& ) = delete;

      private:
         std::string                      m_name;

      public:
         bool insert( const IAsync::tSptr );
         IAsync::tSptr get( );
      private:
         std::deque< IAsync::tSptr >      m_collection;
         os::ConditionVariable            m_buffer_cond_var;

      public:
         void dump( ) const;
   };

} // namespace base::async
