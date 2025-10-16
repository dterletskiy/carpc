#pragma once

#include "carpc/runtime/comm/async/event/IEvent.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "TEvent"



namespace carpc::async {

   template< typename _Generator >
   class TEvent : public IEvent
   {
      // using and types
      public:
         using tEvent         = typename _Generator::Config::tEvent;
         using tEventPtr      = typename std::shared_ptr< tEvent >;
         using tConsumer      = typename _Generator::Config::tConsumer;
         using tService       = typename _Generator::Config::tService;
         using tData          = typename _Generator::Config::tData;
         using tDataPtr       = typename std::shared_ptr< tData >;
         using tPassport      = typename _Generator::Config::tPassport;
         using tSignature     = typename _Generator::Config::tSignature;

      // constructors
      protected:
         TEvent( ) = default;
         TEvent( const tSignature& signature )
         {
            mp_passport = tPassport::create( signature );
         }
      public:
         ~TEvent( ) override = default;

      // static functions
      public:
         static const bool set_notification( tConsumer* p_consumer, const tSignature& signature = { } )
         {
            return IEvent::set_notification( p_consumer, tPassport::create( signature ) );
         }
         static const bool clear_notification( tConsumer* p_consumer, const tSignature& signature = { } )
         {
            return IEvent::clear_notification( p_consumer, tPassport::create( signature ) );
         }
         static const bool clear_all_notifications( tConsumer* p_consumer )
         {
            return IEvent::clear_all_notifications( p_consumer, tPassport::create( ) );
         }

      public:
         static std::shared_ptr< tEvent > create( )
         {
            return std::shared_ptr< tEvent >( new tEvent( ) );
         }
         static std::shared_ptr< tEvent > create( const tSignature& signature )
         {
            return std::shared_ptr< tEvent >( new tEvent( signature ) );
         }


      // virual function
      public:
         void process_event( IAsync::IConsumer* p_consumer ) const override
         {
            static_cast< tConsumer* >( p_consumer )->process_event( *this );
         }

      // serialization / deserialization
      public:
         const bool to_stream_t( ipc::tStream& stream ) const override final
         {
            if constexpr( CARPC_IS_IPC_TYPE( tService ) )
            {
               return ipc::serialize( stream, mp_passport, m_context, m_priority, mp_data );
            }

            return false;
         }
         const bool from_stream_t( ipc::tStream& stream ) override final
         {
            if constexpr( CARPC_IS_IPC_TYPE( tService ) )
            {
               return ipc::deserialize( stream, mp_passport, m_context, m_priority, mp_data );
            }

            return false;
         }

      public:
         const bool is_ipc( ) const override { return CARPC_IS_IPC_TYPE( tService ); }

      // passport
      public:
         const tSignature& signature( ) const
         {
            return mp_passport->user_signature( );
         }
         tEventPtr signature( const tSignature& _signature )
         {
            mp_passport = tPassport::create( _signature );
            return std::shared_ptr< tEvent >( shared_from_this( ), this );
         }
         const IAsync::ISignature::tSptr passport( ) const override
         {
            return mp_passport;
         }
      private:
         typename tPassport::tSptr mp_passport = nullptr;

      // data
      public:
         const tDataPtr data( ) const
         {
            return mp_data;
         }
         tEventPtr data( const tData& data )
         {
            mp_data = std::make_shared< tData >( data );
            return std::shared_ptr< tEvent >( shared_from_this( ), this );
         }
         tEventPtr data( const tDataPtr data )
         {
            mp_data = data;
            return std::shared_ptr< tEvent >( shared_from_this( ), this );
         }
      private:
         tDataPtr mp_data = nullptr;

      // context
      public:
         const application::Context& context( ) const override
         {
            return m_context;
         }
      private:
         application::Context m_context = application::Context::current( );

      // priority
      public:
         const tPriority priority( ) const override
         {
            return m_priority;
         }
         tEventPtr priority( const tPriority& value ) override
         {
            m_priority = value;
            return std::shared_ptr< tEvent >( shared_from_this( ), this );
         }
      protected:
         tPriority m_priority = carpc::priority( ePriority::DEFAULT );
   };

} // namespace carpc::async



#undef CLASS_ABBR
