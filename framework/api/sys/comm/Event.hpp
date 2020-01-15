#pragma once

#include "api/sys/common/Types.hpp"
#include "api/sys/common/ByteBufferT.hpp"
#include "Types.hpp"
#include "EventRegistry.hpp"



namespace base {



/****************************************
 *
 * Event_ID declaration
 *
 ***************************************/
class Event_ID
{
public:
   Event_ID( const std::string& );
   ~Event_ID( );

   bool to_buffer( ByteBufferT& ) const;
   bool from_buffer( ByteBufferT& );

public:
   const char* c_str( ) const;

public:
   const bool operator==( const Event_ID& ) const;
   const bool operator!=( const Event_ID& ) const;
   const bool operator>( const Event_ID& ) const;
   const bool operator<( const Event_ID& ) const;

protected:
   std::string    m_id;
};



/****************************************
 *
 * TEventConsumer declaration
 *
 ***************************************/
class IEventConsumer
{
public:
   IEventConsumer( ) { }
   virtual ~IEventConsumer( ) { }
};

template< typename _Generator >
class TEventConsumer
   : public IEventConsumer
{
public:
   using _EventType = typename _Generator::Config::EventType;

   TEventConsumer( ) { }
   ~TEventConsumer( ) override { }

   virtual void process_event( const _EventType& ) = 0;
};



/****************************************
 *
 * Event declaration
 *
 ***************************************/
class Event
{
public:
   Event( const eCommType comm_type = eCommType::ITC );
   Event( const Event& );
   virtual ~Event( );

   static bool set_notification( bool, IEventConsumer*, const Event_ID& );
   static bool send( EventPtr );

public:
   virtual bool is_dsi( ) const = 0;
   virtual bool to_buffer( ByteBufferT& ) const = 0;
   // This function is used to set data from ByteBufferT only for DSI events
   virtual bool data( ByteBufferT& ) = 0;
   virtual bool send( ) = 0;
   virtual void process( IEventConsumer* ) = 0;

public:
   virtual const Event_ID& type_id( ) = 0;
   const eCommType& comm_type( ) const;
protected:
   eCommType   m_comm_type;
};



/****************************************
 *
 * TEvent declaration
 *
 ***************************************/
#define IS_DSI_EVENT false == std::is_same_v< _ServiceType, void >

template< typename _Generator >
class TEvent
   : public Event
   , public std::enable_shared_from_this< TEvent< _Generator > >
{
public:
   using _EventType     = typename _Generator::Config::EventType;
   using _ConsumerType  = typename _Generator::Config::ConsumerType;
   using _DataType      = typename _Generator::Config::DataType;
   using _DataTypePtr   = typename std::shared_ptr< _DataType >;
   using _ServiceType   = typename _Generator::Config::ServiceType;

public:
   TEvent( const eCommType comm_type )
      : Event( comm_type )
   {
      if constexpr( IS_DSI_EVENT )
         m_comm_type = eCommType::IPC;
   }
   TEvent( const _DataType& data, const eCommType comm_type )
      : Event( comm_type )
   {
      if constexpr( IS_DSI_EVENT )
         m_comm_type = eCommType::IPC;

      mp_data = std::make_shared< _DataType >( data );
   }
   virtual ~TEvent( ) { }


public:
   static bool set_notification( bool is_set, _ConsumerType* p_consumer )
   {
      return Event::set_notification( is_set, p_consumer, s_type_id );
   }

   static std::shared_ptr< _EventType > create( const eCommType comm_type = eCommType::ETC )
   {
      return std::make_shared< _EventType >( comm_type );
   }

   static std::shared_ptr< _EventType > create( const _DataType& data, const eCommType comm_type = eCommType::ETC )
   {
      return std::make_shared< _EventType >( data, comm_type );
   }

   static bool send_event( const _DataType& data, const eCommType comm_type = eCommType::ETC )
   {
      return create( data, comm_type )->send( );
   }

public:
   bool is_dsi( ) const override { return IS_DSI_EVENT; }
   bool to_buffer( ByteBufferT& buffer ) const override
   {
      if constexpr( IS_DSI_EVENT )
      {
         if( false == buffer.push( *mp_data ) )
            return false;
         if( false == buffer.push( s_type_id ) )
            return false;
         return true;
      }
      return false;
   }
   bool data( ByteBufferT& buffer ) override
   {
      if constexpr( IS_DSI_EVENT )
      {
         mp_data = std::make_shared< _DataType >( );
         return buffer.pop( *mp_data );
      }
      return false;
   }

   bool send( ) override
   {
      return Event::send( _EventType::shared_from_this( ) );
   }

   void process( IEventConsumer* p_consumer ) override
   {
      static_cast< _ConsumerType* >( p_consumer )->process_event( *this );
   }

public:
   const Event_ID& type_id( ) override { return s_type_id; }
   static Event_ID         s_type_id;

public:
   _DataTypePtr data( ) const { return mp_data; }
   bool data( const _DataType& data )
   {
      mp_data = std::make_shared< _DataType >( data );
      return nullptr != mp_data;
   }
private:
   _DataTypePtr      mp_data;
};



#undef IS_DSI_EVENT



/****************************************
 *
 * TGenerator declaration
 *
 ***************************************/
template< typename _DataType, typename _EventNamespace, typename _ServiceType = void >
class TGenerator
{
private:
   using _Generator     = TGenerator< _DataType, _EventNamespace, _ServiceType >;
   using _EventType     = TEvent< _Generator >;
   using _ConsumerType  = TEventConsumer< _Generator >;

public:
   struct Config
   {
      using EventType      = _EventType;
      using DataType       = _DataType;
      using ConsumerType   = _ConsumerType;
      using ServiceType    = _ServiceType;
   };
};



template< typename TYPE >
EventPtr create_event( const eCommType comm_type = eCommType::ETC )
{
   return std::make_shared< TYPE >( comm_type );
}

template< typename TYPE >
EventPtr create_event( const typename TYPE::_DataType data, const eCommType comm_type = eCommType::ETC )
{
   return std::make_shared< TYPE >( data, comm_type );
}



} // namespace base



#define DECLARE_EVENT( eventType, dataType ) \
   namespace eventType { \
      class eventType; \
      using Event       = base::TGenerator< dataType, eventType >::Config::EventType; \
      using Consumer    = Event::_ConsumerType; \
      using Data        = dataType; \
   }

#define INIT_EVENT( eventType ) \
   template< > base::Event_ID eventType::Event::s_type_id = { #eventType };

#define REGISTER_EVENT( eventType ) \
   base::EventRegistry::instance( )->register_event( #eventType, base::create_event< eventType::Event > );



#define DECLARE_DSI_EVENT( serviceName, eventType, dataType ) \
   namespace serviceName::eventType { \
      class serviceName; \
      class eventType; \
      using Event       = base::TGenerator< dataType, eventType, serviceName >::Config::EventType; \
      using Consumer    = Event::_ConsumerType; \
      using Data        = dataType; \
   }

#define INIT_DSI_EVENT( serviceName, eventType ) \
      template< > base::Event_ID serviceName::eventType::Event::s_type_id = { #eventType"."#serviceName };

#define REGISTER_DSI_EVENT( serviceName, eventType ) \
   base::EventRegistry::instance( )->register_event( #eventType"."#serviceName, base::create_event< serviceName::eventType::Event > );



#if 0 // Depricated event macros
#define DECLARE_EVENT( eventType, dataType, consumerType ) \
   using eventType      = base::TGenerator< dataType >::Config::EventType; \
   using consumerType   = eventType::_ConsumerType;

#define INIT_EVENT( eventType ) \
   template< > base::Event_ID eventType::s_type_id = { #eventType };

#define REGISTER_EVENT( eventType ) \
   base::EventRegistry::instance( )->register_event( #eventType, base::create_event< eventType > );



#define DECLARE_DSI_EVENT( eventType, dataType, consumerType, serviceName ) \
   namespace serviceName { \
      class serviceName; \
      using eventType      = base::TGenerator< dataType, serviceName >::Config::EventType; \
      using consumerType   = eventType::_ConsumerType; \
   }

#define INIT_DSI_EVENT( eventType, serviceName ) \
   namespace serviceName { \
      template< > base::Event_ID eventType::s_type_id = { #eventType"."#serviceName }; \
   }

#define REGISTER_DSI_EVENT( eventType, serviceName ) \
   base::EventRegistry::instance( )->register_event( #eventType"."#serviceName, base::create_event< serviceName::eventType > );
#endif


