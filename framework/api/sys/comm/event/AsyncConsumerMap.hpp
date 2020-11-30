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
            bool operator( )( const IAsync::ISignature* p_es1, const IAsync::ISignature* p_es2 ) const
            { return *p_es1 < *p_es2; }
         };
         using tConsumersSet = std::set< IAsync::IConsumer* >;
         using tAsyncConsumersMap = std::map< const IAsync::ISignature*, tConsumersSet, Comparator >;

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
         // This method is depricated and should not be used to notify consumers because of returned collection
         // could be modified during this process.
         // This issue could be solved by copying this collection before notifying consumers but
         // this solution is not optimyzed.
         // For most optimyzed solution methods 'start_process' and 'finish_process' should be used.
         // In this case there is no any copy or move collection and it is safe for modifications.
         const tConsumersSet& consumers( const IAsync::ISignature* ) const;
      private:
         tAsyncConsumersMap               m_map;

      public:
         const tConsumersSet& start_process( const IAsync::ISignature* );
         bool finish_process( );
      private:
         class ProcessingSignature
         {
            public:
               using tOpt = std::optional< ProcessingSignature >;

               ProcessingSignature( const std::string& );

               const tConsumersSet& start( const IAsync::ISignature*, tConsumersSet& );
               bool finish( tConsumersSet& );
               void reset( );
               bool is_processing( ) const;
               bool is_processing( const IAsync::ISignature& ) const;
               bool is_processing( const IAsync::ISignature* ) const;

            public:
               const IAsync::ISignature* signature( ) const;
               bool add_consumer_to_remove( IAsync::IConsumer* );
               bool remove_consumer_to_remove( IAsync::IConsumer* );
            private:
               const IAsync::ISignature*     mp_signature = nullptr;
               tConsumersSet                 m_consumers_to_process;
               tConsumersSet                 m_consumers_to_remove;
               const std::string&            m_name;
         };
         ProcessingSignature              m_processing;

      public:
         void dump( ) const;
   };

} // namespace base::async
