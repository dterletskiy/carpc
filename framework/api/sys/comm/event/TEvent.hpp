#pragma once

#include "api/sys/application/Context.hpp"
#include "api/sys/comm/event/IEvent.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TEvent"



namespace base::async {



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
         TEvent( const tSignature& signature, const tPriority& priority )
            : IEvent( )
            , m_signature( signature )
            , m_context( application::Context::eInitType::Auto )
            , m_priority( priority )
         { }
         TEvent( const tSignature& signature, const tData& data, const tPriority& priority )
            : IEvent( )
            , m_signature( signature )
            , m_context( application::Context::eInitType::Auto )
            , m_priority( priority )
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

      public:
         // This "create" static function is used only for creating concrete event type with all empty data
         // what should be filled during serialization.
         static IEvent::tSptr create( )
         {
            return std::shared_ptr< tEvent >( new tEvent( tSignature( tUserSignature( ) ), tPriority( ) ) );
         }

         static std::shared_ptr< tEvent > create(
               const tUserSignature& signature,
               const tPriority& priority = base::priority( ePriority::DEFAULT )
            )
         {
            return std::shared_ptr< tEvent >( new tEvent( tSignature( signature ), priority ) );
         }
         static std::shared_ptr< tEvent > create(
               const tUserSignature& signature,
               const tData& data,
               const tPriority& priority = base::priority( ePriority::DEFAULT )
            )
         {
            return std::shared_ptr< tEvent >( new tEvent( tSignature( signature ), data, priority ) );
         }

         static const bool create_send(
               const tUserSignature& signature,
               const application::Context& to_context = application::Context( )
            )
         {
            return create( signature )->send( to_context );
         }
         static const bool create_send(
               const tUserSignature& signature,
               const tPriority& priority,
               const application::Context& to_context = application::Context( )
            )
         {
            return create( signature, priority )->send( to_context );
         }
         static const bool create_send(
               const tUserSignature& signature,
               const tData& data,
               const application::Context& to_context = application::Context( )
            )
         {
            return create( signature, data )->send( to_context );
         }
         static const bool create_send(
               const tUserSignature& signature,
               const tData& data,
               const tPriority& priority,
               const application::Context& to_context = application::Context( )
            )
         {
            return create( signature, data, priority )->send( to_context );
         }

      // virual function
      public:
         const bool send( const application::Context& to_context = application::Context( ) ) override
         {
            return IEvent::send( TEvent< _Generator >::shared_from_this( ), to_context );
         }
         void process_event( IAsync::IConsumer* p_consumer ) const override
         {
            static_cast< tConsumer* >( p_consumer )->process_event( *this );
         }

      // serialization / deserialization
      public:
         const bool to_stream( dsi::tByteStream& stream ) const override final
         {
            if constexpr( false == std::is_same_v< tService, NoServiceType > )
            {
               if( false == stream.push( m_signature, m_context, m_priority ) )
                  return false;

               if( false == stream.push( ( nullptr != mp_data ) ) )
                  return false;

               if( mp_data )
                  if( false == stream.push( *mp_data ) )
                     return false;

               return true;
            }

            return false;
         }
         const bool from_stream( dsi::tByteStream& stream ) override final
         {
            if constexpr( false == std::is_same_v< tService, NoServiceType > )
            {
               if( false == stream.pop( m_signature, m_context, m_priority ) )
                  return false;

               bool is_data = false;
               if( false == stream.pop( is_data ) )
                  return false;
               if( false == is_data )
                  return true;
               mp_data = std::make_shared< tData >( );
               return stream.pop( *mp_data );
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

      public:
         const application::Context& context( ) const override { return m_context; }
      private:
         application::Context m_context;

      public:
         const tPriority priority( ) const override { return m_priority; }
      protected:
         tPriority m_priority = base::priority( ePriority::DEFAULT );
   };



} // namespace base::async



#undef CLASS_ABBR
