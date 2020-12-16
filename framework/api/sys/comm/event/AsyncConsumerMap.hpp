#pragma once

#include "api/sys/comm/event/IAsync.hpp"



namespace base::async {

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
         const tConsumersSet& start_process( const IAsync::ISignature::tSptr );
         bool finish_process( );
      private:
         class ProcessingSignature
         {
            public:
               using tOpt = std::optional< ProcessingSignature >;

               ProcessingSignature( const std::string& );

               const tConsumersSet& start( const IAsync::ISignature::tSptr, tConsumersSet& );
               bool finish( tConsumersSet& );
               void reset( );
               bool is_processing( ) const;
               bool is_processing( const IAsync::ISignature::tSptr ) const;

            public:
               const IAsync::ISignature::tSptr signature( ) const;
               bool add_consumer_to_remove( IAsync::IConsumer* );
               bool remove_consumer_to_remove( IAsync::IConsumer* );
            private:
               IAsync::ISignature::tSptr     mp_signature = nullptr;
               tConsumersSet                 m_consumers_to_process;
               tConsumersSet                 m_consumers_to_remove;
               const std::string&            m_name;
         };
         ProcessingSignature              m_processing;

      public:
         void dump( ) const;
   };

} // namespace base::async
