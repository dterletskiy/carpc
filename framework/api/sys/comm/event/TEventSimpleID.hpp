#pragma once

#include "api/sys/comm/event/TEventBase.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TEventSimpleID"



namespace base {



#define IS_IPC_EVENT ( false == std::is_same_v< _ServiceType, NoServiceType > )



/****************************************
 *
 * TEventSimpleID declaration
 *
 ***************************************/
template< typename _Generator >
class TEventSimpleID
   : public TEventBase< _Generator >
{
// using and types
public:
   using _TEventBase          = TEventBase< _Generator >;
   using _EventType           = typename _Generator::Config::EventType;
   using _ConsumerType        = typename _Generator::Config::ConsumerType;
   using _DataType            = typename _Generator::Config::DataType;
   using _IdType              = typename _Generator::Config::IdType;
   using _ServiceType         = typename _Generator::Config::ServiceType;

public:
   class Signature : public _TEventBase::Signature
   {
   public:
      static const eEventType build_type( ) { return eEventType::SIMPLE_ID; }

   public:
      Signature( ) : _TEventBase::Signature( ) { }
      Signature( const _IdType& id ) : _TEventBase::Signature( ), m_id( id ) { }
      Signature( const Signature& other ) : _TEventBase::Signature( other ), m_id( other.m_id ) { }
      ~Signature( ) override = default;
      const IEventSignature* const create_copy( ) const { return new Signature( *this ); }

   public:
      const std::string name( ) const override
      {
         return base::format_string( _TEventBase::Signature::name( ), ", id: ", (size_t)m_id );
      }

   public:
      bool operator<( const IEventSignature& signature ) const override
      {
         if( signature.type_id( ) != _TEventBase::Signature::s_type_id )
            return _TEventBase::Signature::s_type_id < signature.type_id( );

         if( signature.type( ) != _TEventBase::Signature::s_type_event )
            return _TEventBase::Signature::s_type_event < signature.type( );

         return m_id < static_cast< const Signature& >( signature ).m_id;
      }

   public:
      const bool to_buffer( ByteBufferT& buffer ) const override
      {
         if constexpr( IS_IPC_EVENT )
            return buffer.push( m_id );
         return false;
      }
      const bool from_buffer( ByteBufferT& buffer ) override
      {
         if constexpr( IS_IPC_EVENT )
            return buffer.pop( m_id );
         return false;
      }

   public:
      const _IdType& id( ) const { return m_id; }
   private:
      _IdType m_id;
   };

// constructors
private:
   TEventSimpleID( const Signature& signature, const eCommType comm_type )
      : _TEventBase( comm_type )
      , m_signature( signature )
   { }
   TEventSimpleID( const Signature& signature, const _DataType& data, const eCommType comm_type )
      : _TEventBase( data, comm_type )
      , m_signature( signature )
   { }
public:
   ~TEventSimpleID( ) override = default;

// static functions
public:
   static const bool set_notification( _ConsumerType* p_consumer, const Signature& signature )
   {
      return IEvent::set_notification( p_consumer, signature );
   }

   static const bool clear_notification( _ConsumerType* p_consumer, const Signature& signature )
   {
      return IEvent::clear_notification( p_consumer, signature );
   }

   static const bool clear_all_notifications( _ConsumerType* p_consumer )
   {
      return IEvent::clear_all_notifications( p_consumer, Signature( ) );
   }

   static std::shared_ptr< _EventType > create( const Signature& signature, const eCommType comm_type = eCommType::NONE )
   {
      return std::shared_ptr< _EventType >( new _EventType( signature, comm_type ) );
   }

   static std::shared_ptr< _EventType > create( const Signature& signature, const _DataType& data, const eCommType comm_type = eCommType::NONE )
   {
      return std::shared_ptr< _EventType >( new _EventType( signature, data, comm_type ) );
   }

   static const bool create_send( const Signature& signature, const eCommType comm_type = eCommType::NONE )
   {
      return create( signature, comm_type )->send( comm_type );
   }

   static const bool create_send( const Signature& signature, const _DataType& data, const eCommType comm_type = eCommType::NONE )
   {
      return create( signature, data, comm_type )->send( comm_type );
   }

   static const bool create_send_to_context( const Signature& signature, ServiceThreadPtrW pw_service )
   {
      return create( signature )->send_to_context( pw_service );
   }

   static const bool create_send_to_context( const Signature& signature, const _DataType& data, ServiceThreadPtrW pw_service )
   {
      return create( signature, data )->send_to_context( pw_service );
   }

private:
   // This "create" static function is used only for creating concrete event type with all empty data what should be filled during serrialization.
   // This function could be called only by "create_event" from "EventRegistry" and this is the reason why it is private.
   template< typename T > friend EventPtr create_event( );
   static std::shared_ptr< _EventType > create( const eCommType comm_type = eCommType::NONE )
   {
      return std::shared_ptr< _EventType >( new _EventType( Signature( ), comm_type ) );
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
 * TGeneratorSimpleID declaration
 *
 ***************************************/
template< typename _ServiceType, typename _EventNamespace, typename _DataType, typename _IdType >
class TGeneratorSimpleID
{
private:
   using _Generator     = TGeneratorSimpleID< _ServiceType, _EventNamespace, _DataType, _IdType >;
   using _EventType     = TEventSimpleID< _Generator >;
   using _ConsumerType  = TEventConsumer< _Generator >;

public:
   struct Config
   {
      using EventType            = _EventType;
      using DataType             = _DataType;
      using IdType               = _IdType;
      using ConsumerType         = _ConsumerType;
      using ServiceType          = _ServiceType;
      using EventProcessorType   = void ( _ConsumerType::* )( const _EventType& );
   };
};



} // namespace base
