#pragma once

#include "carpc/runtime/comm/async/IAsync.hpp"



namespace carpc::async {

   class AsyncConsumerMap
   {
      public:
         using tSptr = std::shared_ptr< AsyncConsumerMap >;
         using tWptr = std::weak_ptr< AsyncConsumerMap >;

         using tSignature = IAsync::ISignature;
         using tConsumer = IAsync::IConsumer;

      private:
         struct Comparator
         {
            bool operator( )( const IAsync::ISignature::tSptr p_es1, const IAsync::ISignature::tSptr p_es2 ) const
            { return *p_es1 < *p_es2; }
         };
         using tConsumersSet = std::set< IAsync::IConsumer* >;
         using tAsyncConsumersMap = std::map< const IAsync::ISignature::tSptr, tConsumersSet, Comparator >;

      public:
         AsyncConsumerMap( const std::string& name = "NoName" );
         ~AsyncConsumerMap( );
         AsyncConsumerMap( const AsyncConsumerMap& ) = delete;
         AsyncConsumerMap& operator=( const AsyncConsumerMap& ) = delete;

      private:
         std::string                      m_name;

      public:
         void set_notification( const IAsync::ISignature::tSptr, IAsync::IConsumer* );
         void clear_notification( const IAsync::ISignature::tSptr, IAsync::IConsumer* );
         void clear_all_notifications( const IAsync::ISignature::tSptr, IAsync::IConsumer* );
         bool is_subscribed( const IAsync::ISignature::tSptr );
      private:
         tAsyncConsumersMap               m_map;

      public:
         bool process( const IAsync::tSptr, std::atomic< time_t >& );
      public:
         IAsync::tSptr                    mp_processing_async = nullptr;
         tConsumersSet                    m_consumers_to_add;
         tConsumersSet                    m_consumers_to_remove;

      public:
         void dump( ) const;
   };

} // namespace carpc::async
