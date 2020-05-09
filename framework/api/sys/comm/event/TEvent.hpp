#pragma once

#include "api/sys/comm/event/IEvent.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TEvent"



namespace base {



   template< typename _Generator >
   class TEvent : public IEvent
   {
      // using and types
      public:
         using tEvent         = typename _Generator::Config::tEvent;
         using tConsumer      = typename _Generator::Config::tConsumer;
         using tService       = typename _Generator::Config::tService;
         using tData          = typename _Generator::Config::tData;
         using tDataPtr       = typename std::shared_ptr< tData >;
         using tSignature     = typename _Generator::Config::tSignature;
         using tUserSignature = typename _Generator::Config::tUserSignature;

      // constructors
      protected:
         TEvent( const tUserSignature& signature, const eCommType comm_type )
            : IEvent( comm_type )
            , m_signature( signature )
         { }
         TEvent( const tUserSignature& signature, const tData& data, const eCommType comm_type )
            : IEvent( comm_type )
            , m_signature( signature )
         {
            mp_data = std::make_shared< tData >( data );
         }
      public:
         ~TEvent( ) override = default;

      // static functions
      public:
         static const bool set_notification( tConsumer* p_consumer, const tUserSignature& signature )
         {
            return IEvent::set_notification( p_consumer, tSignature( signature ) );
         }
         static const bool clear_notification( tConsumer* p_consumer, const tUserSignature& signature )
         {
            return IEvent::clear_notification( p_consumer, tSignature( signature ) );
         }
         static const bool clear_all_notifications( tConsumer* p_consumer )
         {
            return IEvent::clear_all_notifications( p_consumer, tSignature( ) );
         }

         static std::shared_ptr< tEvent > create( const tUserSignature& signature, const eCommType comm_type = eCommType::NONE )
         {
            return std::shared_ptr< tEvent >( new tEvent( signature, comm_type ) );
         }
         static std::shared_ptr< tEvent > create( const tUserSignature& signature, const tData& data, const eCommType comm_type = eCommType::NONE )
         {
            return std::shared_ptr< tEvent >( new tEvent( signature, data, comm_type ) );
         }

         static const bool create_send( const tUserSignature& signature, const eCommType comm_type = eCommType::NONE )
         {
            return create( signature, comm_type )->send( comm_type );
         }
         static const bool create_send( const tUserSignature& signature, const tData& data, const eCommType comm_type = eCommType::NONE )
         {
            return create( signature, data, comm_type )->send( comm_type );
         }

         static const bool create_send_to_context( const tUserSignature& signature, IServiceThread::tWptr pw_service )
         {
            return create( signature )->send_to_context( pw_service );
         }
         static const bool create_send_to_context( const tUserSignature& signature, const tData& data, IServiceThread::tWptr pw_service )
         {
            return create( signature, data )->send_to_context( pw_service );
         }

      private:
         // This "create" static function is used only for creating concrete event type with all empty data what should be filled during serrialization.
         // This function could be called only by "create_event" from "EventRegistry" and this is the reason why it is private.
         template< typename T > friend IEvent::tSptr create_event( );
         static std::shared_ptr< tEvent > create( const eCommType comm_type = eCommType::NONE )
         {
            return std::shared_ptr< tEvent >( new tEvent( tUserSignature( ), comm_type ) );
         }

      // virual function
      public:
         const bool send( const eCommType comm_type = eCommType::NONE ) override
         {
            return IEvent::send( TEvent< _Generator >::shared_from_this( ), comm_type );
         }
         const bool send_to_context( IServiceThread::tWptr pw_service ) override
         {
            return IEvent::send_to_context( TEvent< _Generator >::shared_from_this( ), pw_service );
         }
         void process_event( IAsync::IConsumer* p_consumer ) const override
         {
            static_cast< tConsumer* >( p_consumer )->process_event( *this );
         }

      // serialization / deserialization
      public:
         const bool to_buffer( ByteBufferT& buffer ) const override final
         {
            if constexpr( false == std::is_same_v< tService, NoServiceType > )
            {
               if( mp_data )
               {
                  if( false == buffer.push( *mp_data ) )
                     return false;
               }

               if( false == buffer.push( ( nullptr != mp_data ) ) )
                  return false;

               return buffer.push( m_signature );
            }

            return false;
         }
         const bool from_buffer( ByteBufferT& buffer ) override final
         {
            if constexpr( false == std::is_same_v< tService, NoServiceType > )
            {
               if( false == buffer.pop( m_signature ) )
                  return false;

               bool is_data = false;
               if( false == buffer.pop( is_data ) )
                  return false;
               if( false == is_data )
                  return true;
               mp_data = std::make_shared< tData >( );
               return buffer.pop( *mp_data );
            }

            return false;
         }

      public:
         const bool is_ipc( ) const override { return false == std::is_same_v< tService, NoServiceType >; }

      // signature
      public:
         const tUserSignature& info( ) const { return m_signature.user_signature( ); }
         const IAsync::ISignature* const signature( ) const override { return &m_signature; }
      private:
         tSignature m_signature;

      // data
      public:
         tDataPtr data( ) const { return mp_data; }
         void data( const tData& data ) { mp_data = std::make_shared< tData >( data ); }
         void data( const tDataPtr data ) { mp_data = data; }
      private:
         tDataPtr mp_data = nullptr;
   };



} // namespace base



#undef CLASS_ABBR
