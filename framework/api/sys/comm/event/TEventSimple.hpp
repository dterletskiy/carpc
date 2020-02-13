#pragma once

#include "api/sys/comm/event/TEventBase.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TEventSimple"



namespace base {



#define IS_IPC_EVENT ( false == std::is_same_v< _ServiceType, NoServiceType > )



/****************************************
 *
 * TEventSimple declaration
 *
 ***************************************/
template< typename _Generator >
class TEventSimple
   : public TEventBase< _Generator >
{
// using and types
public:
   using _TEventBase          = TEventBase< _Generator >;
   using _EventType           = typename _Generator::Config::EventType;
   using _ConsumerType        = typename _Generator::Config::ConsumerType;
   using _DataType            = typename _Generator::Config::DataType;
   using _ServiceType         = typename _Generator::Config::ServiceType;

public:
   class Signature : public _TEventBase::Signature
   {
   public:
      static const eEventType build_type( ) { return eEventType::SIMPLE; }

   public:
      Signature( ) : _TEventBase::Signature( ) { }
      Signature( const Signature& other ) : _TEventBase::Signature( other ) { }
      ~Signature( ) override { }
      const IEventSignature* const create_copy( ) const { return new Signature( *this ); }

   public:
      bool operator==( const IEventSignature& signature ) const override
      {
         if( signature.type_id( ) != _TEventBase::Signature::s_type_id )
            return false;
         if( signature.type( ) != _TEventBase::Signature::s_type_event )
            return false;
         return true;
      }

      bool operator<( const IEventSignature& signature ) const override
      {
         if( signature.type_id( ) != _TEventBase::Signature::s_type_id )
            return _TEventBase::Signature::s_type_id < signature.type_id( );

         return _TEventBase::Signature::s_type_event < signature.type( );
      }

   public:
      const bool to_buffer( ByteBufferT& buffer ) const override
      {
         if constexpr( IS_IPC_EVENT )
            return true;
         return false;
      }
      const bool from_buffer( ByteBufferT& buffer ) override
      {
         if constexpr( IS_IPC_EVENT )
            return true;
         return false;
      }
   };

// constructors
private:
   TEventSimple( const eCommType comm_type )
      : _TEventBase( comm_type )
   { }
   TEventSimple( const _DataType& data, const eCommType comm_type )
      : _TEventBase( data, comm_type )
   { }
public:
   ~TEventSimple( ) override = default;

// static functions
public:
   static const bool set_notification( _ConsumerType* p_consumer )
   {
      return IEvent::set_notification( p_consumer, Signature( ) );
   }

   static const bool clear_notification( _ConsumerType* p_consumer )
   {
      return IEvent::clear_notification( p_consumer, Signature( ) );
   }

   static const bool clear_all_notifications( _ConsumerType* p_consumer )
   {
      return IEvent::clear_all_notifications( p_consumer, Signature( ) );
   }

   static std::shared_ptr< _EventType > create( const _DataType& data, const eCommType comm_type = eCommType::NONE )
   {
      return std::shared_ptr< _EventType >( new _EventType( data, comm_type ) );
   }

   static const bool create_send( const eCommType comm_type = eCommType::NONE )
   {
      return create( comm_type )->send( comm_type );
   }

   static const bool create_send( const _DataType& data, const eCommType comm_type = eCommType::NONE )
   {
      return create( data, comm_type )->send( comm_type );
   }

   static const bool create_send_to_context( ServiceThreadPtrW pw_service )
   {
      return create( )->send_to_context( pw_service );
   }

   static const bool create_send_to_context( const _DataType& data, ServiceThreadPtrW pw_service )
   {
      return create( data )->send_to_context( pw_service );
   }

private:
   // This "create" static function is used only for creating concrete event type with all empty data what should be filled during serrialization.
   // This function could be called only by "create_event" from "EventRegistry" and this is the reason why it is private.
   template< typename T > friend EventPtr create_event( );
   static std::shared_ptr< _EventType > create( const eCommType comm_type = eCommType::NONE )
   {
      return std::shared_ptr< _EventType >( new _EventType( comm_type ) );
   }

// serialization / deserialization
private:
   const bool serialize( ByteBufferT& buffer ) const override
   {
      if constexpr( IS_IPC_EVENT )
      {
         return buffer.push( m_signature );
      }
      return false;
   }
   const bool deserialize( ByteBufferT& buffer ) override
   {
      if constexpr( IS_IPC_EVENT )
      {
         return buffer.pop( m_signature );
      }
      return false;
   }

   void process( IEventConsumer* p_consumer ) override
   {
      static_cast< _ConsumerType* >( p_consumer )->process_event( *this );
   }

public:
   const IEventSignature* const signature( ) const override { return &m_signature; }
   const Signature& info( ) const { return m_signature; }
private:
   Signature m_signature;
};



#undef IS_IPC_EVENT
#undef CLASS_ABBR



/****************************************
 *
 * TGeneratorSimple declaration
 *
 ***************************************/
template< typename _ServiceType, typename _EventNamespace, typename _DataType >
class TGeneratorSimple
{
private:
   using _Generator     = TGeneratorSimple< _ServiceType, _EventNamespace, _DataType >;
   using _EventType     = TEventSimple< _Generator >;
   using _ConsumerType  = TEventConsumer< _Generator >;

public:
   struct Config
   {
      using EventType            = _EventType;
      using DataType             = _DataType;
      using ConsumerType         = _ConsumerType;
      using ServiceType          = _ServiceType;
      using EventProcessorType   = void ( _ConsumerType::* )( const _EventType& );
   };
};



} // namespace base
