#pragma once

#include "api/sys/oswrappers/ConditionVariable.hpp"
#include "api/sys/comm/async/IAsync.hpp"



namespace base::async {

   class AsyncPriorityQueue
   {
      public:
         using tSptr = std::shared_ptr< AsyncPriorityQueue >;
         using tWptr = std::weak_ptr< AsyncPriorityQueue >;

      public:
         /***************
          *
          * max_priority - max number of priorities.
          *    Container with prioritised object will containe indexes [0; max_priority),
          *    but real iteration will be performed from (0; max_priority).
          *    This is because of method base::Tpriority::check( )
          *
          **************/
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
