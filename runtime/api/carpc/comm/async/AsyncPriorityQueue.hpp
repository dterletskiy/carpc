#pragma once

#include "carpc/oswrappers/ConditionVariable.hpp"
#include "carpc/comm/async/IAsync.hpp"



namespace carpc::async {

   class AsyncPriorityQueue
   {
      public:
         using tSptr = std::shared_ptr< AsyncPriorityQueue >;
         using tWptr = std::weak_ptr< AsyncPriorityQueue >;
         using tCollection = std::vector< std::deque< IAsync::tSptr > >;

      public:
         /***************
          *
          * max_priority - max number of priorities.
          *    Container with prioritised object will containe indexes [0; max_priority),
          *    but real iteration will be performed from (0; max_priority).
          *    This is because of method carpc::Tpriority::check( )
          *
          **************/
         AsyncPriorityQueue( const tPriority& max_priority = tPriority::max, const std::string& name = "NoName" );
         ~AsyncPriorityQueue( );
         AsyncPriorityQueue( const AsyncPriorityQueue& ) = delete;
         AsyncPriorityQueue& operator=( const AsyncPriorityQueue& ) = delete;

      private:
         std::string                      m_name;

      public:
         bool insert( const IAsync::tSptr );
         IAsync::tSptr get( );
         void clear( );
         void freeze( );
         void unfreeze( );
      private:
         tCollection                m_collections;
         os::ConditionVariable      m_buffer_cond_var;
         std::atomic< bool >        m_freezed = false;

      public:
         void dump( ) const;
   };

} // namespace carpc::async
