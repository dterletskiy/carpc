#pragma once

#include "api/sys/comm/event/IEventSignature.hpp"
#include "api/sys/comm/event/IEvent.hpp"



namespace base {



#define IS_IPC_EVENT ( false == std::is_same_v< _ServiceType, NoServiceType > )



template< typename _Generator >
class TEventBase
   : public IEvent
   , public std::enable_shared_from_this< TEventBase< _Generator > >
{
// using and types
public:
   using _ServiceType         = typename _Generator::Config::ServiceType;
   using _DataType            = typename _Generator::Config::DataType;
   using _DataTypePtr         = typename std::shared_ptr< _DataType >;

public:
   class Signature : public IEventSignature
   {
   public:
      Signature( ) = default;
      Signature( const Signature& ) = default;
      ~Signature( ) override = default;

   public:
      const std::string name( ) const override
      {
         static const std::string s_name = base::format_string( s_type_id, "/", base::c_str( s_type_event ) );
         return s_name;
      }

   public:
      const EventTypeID& type_id( ) const override final { return s_type_id; }
      const eEventType type( ) const override final { return s_type_event; }

   public:
      static const EventTypeID   s_type_id;
      static const eEventType    s_type_event;
   };

// constructors
protected:
   TEventBase( const eCommType comm_type )
      : IEvent( comm_type )
   {
   }
   TEventBase( const _DataType& data, const eCommType comm_type )
      : IEvent( comm_type )
   {
      mp_data = std::make_shared< _DataType >( data );
   }
   ~TEventBase( ) override = default;

// virual function
public:
   const bool send( const eCommType comm_type = eCommType::NONE ) override
   {
      return IEvent::send( TEventBase< _Generator >::shared_from_this( ), comm_type );
   }
   const bool send_to_context( ServiceThreadPtrW pw_service ) override
   {
      return IEvent::send_to_context( TEventBase< _Generator >::shared_from_this( ), pw_service );
   }

// serialization / deserialization
public:
   virtual const bool serialize( ByteBufferT& ) const = 0;
   virtual const bool deserialize( ByteBufferT& ) = 0;
   const bool to_buffer( ByteBufferT& buffer ) const override final
   {
      if constexpr( IS_IPC_EVENT )
      {
         if( mp_data )
         {
            if( false == buffer.push( *mp_data ) )
               return false;
         }

         if( false == buffer.push( ( nullptr != mp_data ) ) )
            return false;

         return serialize( buffer );
      }
      return false;
   }
   const bool from_buffer( ByteBufferT& buffer ) override final
   {
      if constexpr( IS_IPC_EVENT )
      {
         if( false == deserialize( buffer ) )
            return false;

         bool is_data = false;
         if( false == buffer.pop( is_data ) )
            return false;
         if( false == is_data )
            return true;
         mp_data = std::make_shared< _DataType >( );
         return buffer.pop( *mp_data );
      }
      return false;
   }

public:
   const bool is_ipc( ) const override { return IS_IPC_EVENT; }

// data
public:
   _DataTypePtr data( ) const { return mp_data; }
   bool data( const _DataType& data )
   {
      mp_data = std::make_shared< _DataType >( data );
      return nullptr != mp_data;
   }
   bool data( const _DataTypePtr data )
   {
      mp_data = data;
      return nullptr != mp_data;
   }
protected:
   _DataTypePtr mp_data = nullptr; // @TDA-issue: never check for nullptr during serrialization
};



#undef IS_IPC_EVENT



} // namespace base
