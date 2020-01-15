#pragma once

#include "api/sys/comm/IEvent.hpp"



namespace base {



#define IS_DSI_EVENT ( false == std::is_same_v< _ServiceType, NoServiceType > )

template< typename _Generator >
class TEventBase
   : public Event
   , public std::enable_shared_from_this< TEventBase< _Generator > >
{
// using
public:
   using _EventType           = typename _Generator::Config::EventType;
   using _ConsumerType        = typename _Generator::Config::ConsumerType;
   using _DataType            = typename _Generator::Config::DataType;
   using _DataTypePtr         = typename std::shared_ptr< _DataType >;
   using _ServiceType         = typename _Generator::Config::ServiceType;
   using _EventProcessorPtr   = void ( _ConsumerType::* )( const _EventType& );

// constructors
public:
   TEventBase( const eCommType comm_type )
      : Event( )
      , m_comm_type( comm_type )
   {
   }
   TEventBase( const _DataType& data, const eCommType comm_type )
      : Event( )
      , m_comm_type( comm_type )
   {
      mp_data = std::make_shared< _DataType >( data );
   }
   ~TEventBase( ) override { }

// virual function
public:
   const bool is_dsi( ) const override { return IS_DSI_EVENT; }

   const bool send( const eCommType comm_type = eCommType::NONE ) override
   {
      return Event::send( TEventBase< _Generator >::shared_from_this( ), comm_type );
   }
   const bool send_to_context( ServicePtrW pw_service ) override
   {
      return Event::send_to_context( TEventBase< _Generator >::shared_from_this( ), pw_service );
   }

// s_type_id
public:
   const std::string name( ) const override { return s_type_id; }
   const EventTypeID& type_id( ) const override { return s_type_id; }
protected:
   static EventTypeID      s_type_id;

// m_comm_type
public:
   const eCommType& comm_type( ) const override { return m_comm_type; }
private:
   eCommType               m_comm_type;

// data
public:
   _DataTypePtr data( ) const { return mp_data; }
   bool data( const _DataType& data )
   {
      mp_data = std::make_shared< _DataType >( data );
      return nullptr != mp_data;
   }
protected:
   _DataTypePtr            mp_data = nullptr; // @TDA-issue: newer check for nullptr during serrialization
};



#undef IS_DSI_EVENT



} // namespace base
