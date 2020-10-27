#pragma once

#include "api/sys/comm/event/IAsync.hpp"



namespace base::async {

   class AsyncConsumerMap
   {
      public:
         using tSptr = std::shared_ptr< AsyncConsumerMap >;
         using tWptr = std::weak_ptr< AsyncConsumerMap >;

      private:
         struct Comparator
         {
            bool operator( )( const IAsync::ISignature* p_es1, const IAsync::ISignature* p_es2 ) const
            { return *p_es1 < *p_es2; }
         };
         using tAsyncConsumersMap = std::map< const IAsync::ISignature*, std::set< IAsync::IConsumer* >, Comparator >;

      public:
         AsyncConsumerMap( const std::string& name = "NoName" );
         ~AsyncConsumerMap( );
         AsyncConsumerMap( const AsyncConsumerMap& ) = delete;
         AsyncConsumerMap& operator=( const AsyncConsumerMap& ) = delete;

      private:
         std::string                      m_name;

      public:
         void set_notification( const IAsync::ISignature&, IAsync::IConsumer* );
         void clear_notification( const IAsync::ISignature&, IAsync::IConsumer* );
         void clear_all_notifications( const IAsync::ISignature&, IAsync::IConsumer* );
         bool is_subscribed( const IAsync::ISignature* );
         const std::set< IAsync::IConsumer* >& consumers( const IAsync::ISignature* ) const;
      private:
         tAsyncConsumersMap               m_map;

      public:
         void dump( ) const;
   };

} // namespace base::async
