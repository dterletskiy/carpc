#pragma once

#include "api/sys/comm/event/TEventBase.hpp"



namespace base {



#define IS_DSI_EVENT ( false == std::is_same_v< _ServiceType, NoServiceType > )



template< typename _Generator, typename Enable = void >
class TEvent;



template< typename _Generator >
class TEvent< _Generator, std::enable_if_t< std::is_same< typename _Generator::Config::IdType, NoIdInfoType >::value > >
   : public TEventBase< _Generator >
{
// using
public:
   using _TEventBase          = TEventBase< _Generator >;
   using _EventType           = typename _Generator::Config::EventType;
   using _ConsumerType        = typename _Generator::Config::ConsumerType;
   using _DataType            = typename _Generator::Config::DataType;
   using _DataTypePtr         = typename std::shared_ptr< _DataType >;
   using _IdType              = typename _Generator::Config::IdType;
   using _ServiceType         = typename _Generator::Config::ServiceType;
   using _EventProcessorPtr   = void ( _ConsumerType::* )( const _EventType& );

// constructors
public:
   TEvent( const eCommType comm_type )
      : _TEventBase( comm_type )
   {
   }
   TEvent( const _DataType& data, const eCommType comm_type )
      : _TEventBase( data, comm_type )
   {
   }
   ~TEvent( ) override = default;

// static functions
public:
   static const bool set_notification( bool is_set, _ConsumerType* p_consumer )
   {
      return IEvent::set_notification( is_set, p_consumer, _TEventBase::s_type_id, std::nullopt );
   }

   static const bool clear_all_notifications( _ConsumerType* p_consumer )
   {
      return IEvent::clear_all_notifications( p_consumer, _TEventBase::s_type_id );
   }

   static std::shared_ptr< _EventType > create( const eCommType comm_type = eCommType::ETC )
   {
      return std::make_shared< _EventType >( comm_type );
   }

   static std::shared_ptr< _EventType > create( const _DataType& data, const eCommType comm_type = eCommType::ETC )
   {
      return std::make_shared< _EventType >( data, comm_type );
   }

   static const bool create_send( const eCommType comm_type = eCommType::ETC )
   {
      return create( comm_type )->send( comm_type );
   }

   static const bool create_send( const _DataType& data, const eCommType comm_type = eCommType::ETC )
   {
      return create( data, comm_type )->send( comm_type );
   }

   static const bool create_send_to_context( ServicePtrW pw_service )
   {
      return create( )->send_to_context( pw_service );
   }

   static const bool create_send_to_context( const _DataType& data, ServicePtrW pw_service )
   {
      return create( data )->send_to_context( pw_service );
   }

// serrialization / deserrialization
public:
   const bool to_buffer( ByteBufferT& buffer ) const override
   {
      if constexpr( IS_DSI_EVENT )
      {
         return buffer.push( *_TEventBase::mp_data );
      }
      return false;
   }
   const bool from_buffer( ByteBufferT& buffer ) override
   {
      if constexpr( IS_DSI_EVENT )
      {
         _TEventBase::mp_data = std::make_shared< _DataType >( );
         return buffer.pop( *_TEventBase::mp_data );
      }
      return false;
   }

   void process( IEventConsumer* p_consumer ) override
   {
      static_cast< _ConsumerType* >( p_consumer )->process_event( *this );
   }

// id
public:
   const OptEventInfoID is_broadcast( ) const override { return std::nullopt; }
};



template< typename _Generator >
class TEvent< _Generator, std::enable_if_t< !std::is_same< typename _Generator::Config::IdType, NoIdInfoType >::value > >
   : public TEventBase< _Generator >
{
// using
public:
   using _TEventBase          = TEventBase< _Generator >;
   using _EventType           = typename _Generator::Config::EventType;
   using _ConsumerType        = typename _Generator::Config::ConsumerType;
   using _DataType            = typename _Generator::Config::DataType;
   using _DataTypePtr         = typename std::shared_ptr< _DataType >;
   using _IdType              = typename _Generator::Config::IdType;
   using _ServiceType         = typename _Generator::Config::ServiceType;
   using _EventProcessorPtr   = void ( _ConsumerType::* )( const _EventType& );

// constructors
public:
   TEvent( const eCommType comm_type )
      : _TEventBase( comm_type )
   {
   }
   TEvent( const _IdType id, const eCommType comm_type )
      : _TEventBase( comm_type )
      , m_id( id )
   {
   }
   TEvent( const _DataType& data, const _IdType id, const eCommType comm_type )
      : _TEventBase( data, comm_type )
      , m_id( id )
   {
   }
   ~TEvent( ) override = default;

// static functions
public:
   static const bool set_notification( bool is_set, _ConsumerType* p_consumer, const _IdType id )
   {
      return IEvent::set_notification( is_set, p_consumer, _TEventBase::s_type_id, static_cast< EventInfoID >( id ) );
   }

   static const bool clear_all_notifications( _ConsumerType* p_consumer )
   {
      return IEvent::clear_all_notifications( p_consumer, _TEventBase::s_type_id );
   }

   static std::shared_ptr< _EventType > create( const _IdType id, const eCommType comm_type = eCommType::ETC )
   {
      return std::make_shared< _EventType >( id, comm_type );
   }

   static std::shared_ptr< _EventType > create( const _DataType& data, const _IdType id, const eCommType comm_type = eCommType::ETC )
   {
      return std::make_shared< _EventType >( data, id, comm_type );
   }

   static const bool create_send( const _IdType id, const eCommType comm_type = eCommType::ETC )
   {
      return create( id, comm_type )->send( comm_type );
   }

   static const bool create_send( const _DataType& data, const _IdType id, const eCommType comm_type = eCommType::ETC )
   {
      return create( data, id, comm_type )->send( comm_type );
   }

   static const bool create_send_to_context( const _IdType id, ServicePtrW pw_service )
   {
      return create( id )->send_to_context( pw_service );
   }

   static const bool create_send_to_context( const _DataType& data, const _IdType id, ServicePtrW pw_service )
   {
      return create( data, id )->send_to_context( pw_service );
   }

// serrialization / deserrialization
public:
   const bool to_buffer( ByteBufferT& buffer ) const override
   {
      if constexpr( IS_DSI_EVENT )
      {
         return buffer.push( *_TEventBase::mp_data, m_id );
      }
      return false;
   }
   const bool from_buffer( ByteBufferT& buffer ) override
   {
      if constexpr( IS_DSI_EVENT )
      {
         _TEventBase::mp_data = std::make_shared< _DataType >( );
         return buffer.pop( m_id, *_TEventBase::mp_data );
      }
      return false;
   }

   void process( IEventConsumer* p_consumer ) override
   {
      static_cast< _ConsumerType* >( p_consumer )->process_event( *this );
   }

// id
public:
   const OptEventInfoID is_broadcast( ) const override { return std::make_optional( static_cast< EventInfoID >( m_id ) ); }
   const _IdType& id( ) const { return m_id; }
   void id( const _IdType& id ) { m_id = id; }
private:
   _IdType           m_id;
};



#undef IS_DSI_EVENT



} // namespace base
