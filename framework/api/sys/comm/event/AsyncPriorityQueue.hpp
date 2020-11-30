#pragma once

#include "api/sys/oswrappers/ConditionVariable.hpp"
#include "api/sys/comm/event/IAsync.hpp"



namespace base::async {

   class AsyncPriorityQueue
   {
      public:
         using tSptr = std::shared_ptr< AsyncPriorityQueue >;
         using tWptr = std::weak_ptr< AsyncPriorityQueue >;

      public:
         AsyncPriorityQueue( const tPriority& max_priority = tPriority::max( ), const std::string& name = "NoName" );
         ~AsyncPriorityQueue( );
         AsyncPriorityQueue( const AsyncPriorityQueue& ) = delete;
         AsyncPriorityQueue& operator=( const AsyncPriorityQueue& ) = delete;

      private:
         std::string                      m_name;

      public:
         bool insert( const IAsync::tSptr );
         IAsync::tSptr get( );
      private:
         std::vector< std::deque< IAsync::tSptr > >
                                          m_collections;
         os::ConditionVariable            m_buffer_cond_var;

      public:
         void dump( ) const;
   };

} // namespace base::async
