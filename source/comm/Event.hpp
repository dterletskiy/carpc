#pragma once

#include "common/Types.hpp"
#include "common/ByteBuffer.hpp"
#include "Types.hpp"



namespace base {



/****************************************
 *
 * Event_ID declaration
 *
 ***************************************/
class Event_ID
{
   friend ByteBuffer& operator << ( ByteBuffer&, const Event_ID& );
   friend ByteBuffer& operator >> ( ByteBuffer&, Event_ID& );

public:
   Event_ID( const std::string& );
   ~Event_ID( );

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
   static bool send( EventPtr, const Event_ID& );
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

   TEvent( const _DataType& data, const eCommType comm_type )
      : Event( comm_type )
   {
      mp_data = std::make_shared< _DataType >( data );
   }
   virtual ~TEvent( ) { }

   static bool set_notification( bool is_set, _ConsumerType* p_consumer )
   {
      return Event::set_notification( is_set, p_consumer, s_type_id );
   }

   static std::shared_ptr< _EventType > create( const _DataType& data, const eCommType comm_type = eCommType::ETC )
   {
      eCommType type = comm_type;
      if constexpr( false == std::is_same< _ServiceType, void >::value )
         type = eCommType::IPC;

      return std::make_shared< _EventType >( data, comm_type );
   }

   bool send( ) override
   {
      return Event::send( _EventType::shared_from_this( ), s_type_id );
   }

   static bool send_event( const _DataType& data, const eCommType comm_type = eCommType::ETC )
   {
      return create( data, comm_type )->send( );
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
private:
   _DataTypePtr      mp_data;
};



/****************************************
 *
 * TGenerator declaration
 *
 ***************************************/
template< typename _DataType, typename _ServiceType = void >
class TGenerator
{
private:
   using Generator      = TGenerator< _DataType, _ServiceType >;
   using _EventType     = TEvent< Generator >;
   using _ConsumerType  = TEventConsumer< Generator >;

public:
   struct Config
   {
      using EventType      = _EventType;
      using DataType       = _DataType;
      using ConsumerType   = _ConsumerType;
      using ServiceType    = _ServiceType;
   };
};



} // namespace base



#define DECLARE_EVENT( eventType, dataType, consumerType ) \
   using eventType      = base::TGenerator< dataType >::Config::EventType; \
   using consumerType   = eventType::_ConsumerType;

#define INIT_EVENT( eventType ) \
   template< > base::Event_ID eventType::s_type_id = { #eventType };



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


